// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "jd_control.h"
#include "jd_physical.h"
#include "jd_services.h"
#include "jd_util.h"

#define JD_CLIENT_EV_CONNECT 0x0001
#define JD_CLIENT_EV_DISCONNECT 0x0002
#define JD_CLIENT_EV_BLOCK_CONNECT 0x0003
#define JD_CLIENT_EV_PACKET 0x0004
#define JD_CLIENT_EV_ANY_PACKET 0x0005

typedef struct jd_device jd_device_t;
typedef struct jd_client jd_client_t;

typedef int (*jd_client_cb_t)(jd_client_t *client, int event, jd_packet_t *pkt);

struct jd_client {
    struct jd_client *next_global;
    struct jd_client *next_attached;
    uint32_t service_class;
    uint8_t service_index;
    jd_device_t *device;
    jd_client_cb_t handler;
    void *userdata;
};

void jd_client_process_packet(jd_packet_t *pkt);

#ifdef JD_CLIENT
jd_client_t *jd_client_new(uint32_t service_class, jd_client_cb_t handler);
#endif
