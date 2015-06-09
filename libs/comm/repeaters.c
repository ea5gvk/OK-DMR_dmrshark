#include <config/defaults.h>

#include "repeaters.h"
#include "comm.h"
#include "snmp.h"

#include <libs/daemon/console.h>
#include <libs/daemon/daemon-poll.h>
#include <libs/config/config.h>
#include <libs/remotedb/remotedb.h>

#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

static repeater_t repeaters[MAX_REPEATER_COUNT];

repeater_t *repeaters_findbyip(struct in_addr *ipaddr) {
	int i;

	for (i = 0; i < MAX_REPEATER_COUNT; i++) {
		if (memcmp(&repeaters[i].ipaddr, ipaddr, sizeof(struct in_addr)) == 0)
			return &repeaters[i];
	}
	return NULL;
}

static repeater_t *repeaters_findfirstemptyslot(void) {
	int i;

	for (i = 0; i < MAX_REPEATER_COUNT; i++) {
		if (repeaters[i].ipaddr.s_addr == 0)
			return &repeaters[i];
	}
	return NULL;
}

static flag_t repeaters_issnmpignoredforip(struct in_addr *ipaddr) {
	char *ignoredhosts = config_get_ignoredsnmprepeaterhosts();
	char *tok = NULL;
	struct in_addr ignoredaddr;

	tok = strtok(ignoredhosts, ",");
	if (tok) {
		do {
			if (comm_hostname_to_ip(tok, &ignoredaddr)) {
				if (memcmp(&ignoredaddr, ipaddr, sizeof(struct in_addr)) == 0) {
					free(ignoredhosts);
					return 1;
				}
			} else
				console_log(LOGLEVEL_DEBUG "repeaters: can't resolve hostname %s\n", tok);

			tok = strtok(NULL, ",");
		} while (tok != NULL);
	}
	free(ignoredhosts);
	return 0;
}

repeater_t *repeaters_add(struct in_addr *ipaddr) {
	repeater_t *repeater = repeaters_findbyip(ipaddr);

	if (repeater == NULL) {
		repeater = repeaters_findfirstemptyslot();
		if (repeater == NULL) {
			console_log("repeaters [%s]: can't add new repeater, list is full (%u elements)\n", comm_get_ip_str(ipaddr), MAX_REPEATER_COUNT);
			return NULL;
		}
		memset(repeater, 0, sizeof(repeater_t));
		memcpy(&repeater->ipaddr, ipaddr, sizeof(struct in_addr));
		if (repeaters_issnmpignoredforip(ipaddr))
			repeater->snmpignored = 1;
		console_log("repeaters [%s]: added (snmp ignored: %u)\n", comm_get_ip_str(ipaddr), repeater->snmpignored);
	}
	repeater->last_active_time = time(NULL);
	return repeater;
}

void repeaters_list(void) {
	int i;

	console_log("repeaters:\n");
	console_log("      nr              ip     id  callsign  act  lstinf       type        fwver    dlfreq    ulfreq\n");
	for (i = 0; i < MAX_REPEATER_COUNT; i++) {
		if (repeaters[i].ipaddr.s_addr == 0)
			continue;

		console_log("  #%4u: %15s %6u %9s %4u  %6u %10s %10s %9u %9u %s\n",
			i,
			comm_get_ip_str(&repeaters[i].ipaddr),
			repeaters[i].id,
			repeaters[i].callsign,
			time(NULL)-repeaters[i].last_active_time,
			time(NULL)-repeaters[i].last_snmpinfo_request_time,
			repeaters[i].type,
			repeaters[i].fwversion,
			repeaters[i].dlfreq,
			repeaters[i].ulfreq,
			(repeaters[i].snmpignored ? "snmp ignored" : ""));
	}
}

void repeaters_process(void) {
	int i;
	struct timeval currtime = {0,};
	struct timeval difftime = {0,};

	for (i = 0; i < MAX_REPEATER_COUNT; i++) {
		if (repeaters[i].ipaddr.s_addr == 0)
			continue;

		if (time(NULL)-repeaters[i].last_active_time > config_get_repeaterinactivetimeoutinsec()) {
			console_log("repeaters [%s]: timed out, removing\n", comm_get_ip_str(&repeaters[i].ipaddr));
			memset(&repeaters[i], 0, sizeof(repeater_t));
			continue;
		}

		if (!repeaters[i].snmpignored && time(NULL)-repeaters[i].last_snmpinfo_request_time > config_get_snmpinfoupdateinsec()) {
			console_log(LOGLEVEL_DEBUG "repeaters [%s]: sending snmp info update request\n", comm_get_ip_str(&repeaters[i].ipaddr));
			snmp_start_read_repeaterinfo(comm_get_ip_str(&repeaters[i].ipaddr));
			repeaters[i].last_snmpinfo_request_time = time(NULL);
		}

		if (repeaters[i].slot[0].call_running && time(NULL)-repeaters[i].slot[0].last_packet_received_at > config_get_calltimeoutinsec()) {
			console_log(LOGLEVEL_COMM "repeaters [%s]: call timeout on ts1\n", comm_get_ip_str(&repeaters[i].ipaddr));
			repeaters[i].slot[0].call_running = 0;
			repeaters[i].slot[0].call_ended_at = time(NULL);
			remotedb_update(&repeaters[i]);
		}

		if (repeaters[i].slot[1].call_running && time(NULL)-repeaters[i].slot[1].last_packet_received_at > config_get_calltimeoutinsec()) {
			console_log(LOGLEVEL_COMM "repeaters [%s]: call timeout on ts2\n", comm_get_ip_str(&repeaters[i].ipaddr));
			repeaters[i].slot[1].call_running = 0;
			repeaters[i].slot[1].call_ended_at = time(NULL);
			remotedb_update(&repeaters[i]);
		}

		if (repeaters[i].auto_rssi_update_enabled_at > 0 && repeaters[i].auto_rssi_update_enabled_at <= time(NULL)) {
			if (!repeaters[i].slot[0].call_running && !repeaters[i].slot[1].call_running)
				repeaters[i].auto_rssi_update_enabled_at = 0;
			else {
				gettimeofday(&currtime, NULL);
				timersub(&currtime, &repeaters[i].last_rssi_request_time, &difftime);
				if (difftime.tv_sec*1000+difftime.tv_usec/1000 > config_get_rssiupdateduringcallinmsec()) {
					snmp_start_read_rssi(comm_get_ip_str(&repeaters[i].ipaddr));
					repeaters[i].last_rssi_request_time = currtime;
				}
			}
		}
	}
}

void repeaters_init(void) {
	console_log("repeaters: init\n");

	memset(&repeaters, 0, sizeof(repeater_t)*MAX_REPEATER_COUNT);
}