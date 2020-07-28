#include "jd_control.h"
#include "jd_services.h"
#include "jd_io.h"
#include "interfaces/jd_tx.h"
#include "interfaces/jd_hw.h"

#include "jd_util.h"

#define SERV_NUM 1

struct state {
    uint64_t device_id;
    uint16_t port;
    uint16_t counter;
};

static struct state state;

int jd_evpipe_handle_pkt(jd_packet_t *pkt) {
    if (pkt->service_number == SERV_NUM && pkt->service_command == JD_CMD_PIPE_EVENTS) {
        if (pkt->service_size < sizeof(jd_pipe_cmd_t))
            return -1;
        jd_pipe_cmd_t *cmd = (jd_pipe_cmd_t *)pkt->data;
        state.device_id = cmd->device_identifier;
        state.port = cmd->port_num;
        state.counter = 0;
        return 1;
    }
    return 0;
}

int jd_evpipe_handle_pkt(jd_packet_t *pkt) {
    jd_send_event_ext
}
