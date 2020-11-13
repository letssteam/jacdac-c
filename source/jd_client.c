// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "jd_client.h"

#include "jd_services.h"
#include "jd_control.h"
#include "jd_util.h"
#include "interfaces/jd_app.h"
#include "interfaces/jd_alloc.h"

#include <stdlib.h>

#ifndef JD_CLIENT
void jd_client_process_packet(jd_packet_t *pkt) {}
#else

#define JD_CLIENT_EV_CONNECT 0x0001
#define JD_CLIENT_EV_DISCONNECT 0x0002
#define JD_CLIENT_EV_BLOCK_CONNECT 0x0003
#define JD_CLIENT_EV_PACKET 0x0004
#define JD_CLIENT_EV_ANY_PACKET 0x0005

typedef int (*jd_client_cb_t)(void *userdata, int event, jd_packet_t *pkt);

typedef struct jd_client {
    struct jd_client *next;
    struct jd_client *next_attached;
    uint32_t service_class;
    uint8_t service_index;
    jd_client_cb_t handler;
    void *userdata;
} jd_client_t;
static jd_client_t *clients;

typedef struct jd_device {
    struct jd_device *next;
    uint32_t *services;
    uint64_t device_identifier;
    uint32_t last_seen;
    jd_client_t *attached_clients;
    uint8_t num_services;
} jd_device_t;
static jd_device_t *devices;

static jd_device_t *lookup_device(uint64_t devId, bool alloc) {
    jd_device_t *d;
    for (d = devices; d; d = d->next) {
        if (d->device_identifier == devId)
            return d;
    }

    if (alloc) {
        d = malloc(sizeof(*d));
        memset(d, 0, sizeof(*d));
        d->next = devices;
        d->device_identifier = devId;
        devices = d;
    }

    return d;
}

static bool same_services(jd_device_t *d, jd_packet_t *pkt) {
    if (!d || !d->services)
        return false;

    uint8_t num = d->service_size / 4;
    if (num != d->num_services)
        return false;

    uint32_t *new_services = (uint32_t *)d->data;

    uint8_t cnt0 = d->services[0] & JD_ADVERTISEMENT_0_COUNTER_MASK;
    uint8_t cnt1 = new_services[0] & JD_ADVERTISEMENT_0_COUNTER_MASK;
    if (cnt0 > cnt1)
        return false; // device rebooted

    for (int i = 1; i < num; ++i)
        if (d->services[i] != new_services[i])
            return false;

    return true;
}

void jd_client_process_packet(jd_packet_t *pkt) {
    bool is_announce = pkt->service_command == JD_CTRL_CMD_SERVICES && pkt->service_num == 0;
    jd_device_t *d = lookup_device(pkt->device_identifier, is_announce);

    if (is_announce) {
        if (same_services(d, pkt)) {
            // update reset counter etc.
            d->services[0] = ((uint32_t *)d->data)[0];
        } else {
            jd_client_t *next = d->attached_clients;
            d->attached_clients = NULL;
            for (jd_client_t *c = next; c; c = next) {
                next = c->next_attached;
                c->next_attached = NULL;
                c->handler(c->userdata, JD_CLIENT_EV_DISCONNECT, NULL);
            }

            free(d->services);
            d->num_services = d->service_size / 4;
            d->services = malloc(d->num_services * 4);
            memcpy(d->services, d->data, d->num_services * 4);
        }
    }

    for (jd_client_t *c = clients; c; c = c->next) {
        c->handler(c->userdata, JD_CLIENT_EV_ANY_PACKET, pkt);
    }

    if (!d)
        return;
}

#endif