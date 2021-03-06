/*
 * This file is part of the Soletta (TM) Project
 *
 * Copyright (C) 2015 Intel Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "sol-lwm2m.h"
#include "sol-monitors.h"

#define LWM2M_BOOTSTRAP_QUERY_PARAMS (1)
#define LWM2M_UPDATE_QUERY_PARAMS (4)
#define LWM2M_REGISTER_QUERY_PARAMS (5)
#define NUMBER_OF_PATH_SEGMENTS (3)
#define DEFAULT_SHORT_SERVER_ID (0)
#define DEFAULT_CLIENT_LIFETIME (86400)
#define DEFAULT_BINDING_MODE (SOL_LWM2M_BINDING_MODE_U)
#define DEFAULT_LOCATION_PATH_SIZE (10)
#define TLV_TYPE_MASK (192)
#define TLV_ID_SIZE_MASK (32)
#define TLV_CONTENT_LENGTH_MASK (24)
#define TLV_CONTENT_LENGHT_CUSTOM_MASK (7)
#define REMOVE_SIGN_BIT_MASK (127)
#define SIGN_BIT_MASK (128)
#define ID_HAS_16BITS_MASK (32)
#define OBJ_LINK_LEN (4)
#define LEN_IS_8BITS_MASK (8)
#define LEN_IS_16BITS_MASK (16)
#define LEN_IS_24BITS_MASK (24)
#define UINT24_MAX (16777215)
#define ONE_SECOND (1000)

#define SECURITY_SERVER_OBJECT_ID (0)
#define SECURITY_SERVER_URI (0)
#define SECURITY_SERVER_IS_BOOTSTRAP (1)
#define SECURITY_SERVER_ID (10)
#define SECURITY_SERVER_CLIENT_HOLD_OFF_TIME (11)
#define SECURITY_SERVER_BOOTSTRAP_SERVER_ACCOUNT_TIMEOUT (12)

#define SERVER_OBJECT_ID (1)
#define SERVER_OBJECT_SERVER_ID (0)
#define SERVER_OBJECT_LIFETIME (1)
#define SERVER_OBJECT_BINDING (7)

#define ACCESS_CONTROL_OBJECT_ID (2)
#define ACCESS_CONTROL_OBJECT_OBJECT_RES_ID (0)
#define ACCESS_CONTROL_OBJECT_INSTANCE_RES_ID (1)
#define ACCESS_CONTROL_OBJECT_ACL_RES_ID (2)
#define ACCESS_CONTROL_OBJECT_OWNER_RES_ID (3)

#ifndef SOL_NO_API_VERSION
#define LWM2M_TLV_CHECK_API(_tlv, ...) \
    do { \
        if (SOL_UNLIKELY((_tlv)->api_version != \
            SOL_LWM2M_TLV_API_VERSION)) { \
            SOL_WRN("Couldn't handle tlv that has unsupported version " \
                "'%u', expected version is '%u'", \
                (_tlv)->api_version, SOL_LWM2M_TLV_API_VERSION); \
            return __VA_ARGS__; \
        } \
    } while (0);
#define LWM2M_RESOURCE_CHECK_API(_resource, ...) \
    do { \
        if (SOL_UNLIKELY((_resource)->api_version != \
            SOL_LWM2M_RESOURCE_API_VERSION)) { \
            SOL_WRN("Couldn't handle resource that has unsupported version " \
                "'%u', expected version is '%u'", \
                (_resource)->api_version, SOL_LWM2M_RESOURCE_API_VERSION); \
            return __VA_ARGS__; \
        } \
    } while (0);
#define LWM2M_RESOURCE_CHECK_API_GOTO(_resource, _label) \
    do { \
        if (SOL_UNLIKELY((_resource).api_version != \
            SOL_LWM2M_RESOURCE_API_VERSION)) { \
            SOL_WRN("Couldn't handle resource that has unsupported version " \
                "'%u', expected version is '%u'", \
                (_resource).api_version, SOL_LWM2M_RESOURCE_API_VERSION); \
            goto _label; \
        } \
    } while (0);
#define LWM2M_OBJECT_CHECK_API(_obj, ...) \
    do { \
        if (SOL_UNLIKELY((_obj).api_version != \
            SOL_LWM2M_OBJECT_API_VERSION)) { \
            SOL_WRN("Couldn't handle object that has unsupported version " \
                "'%u', expected version is '%u'", \
                (_obj).api_version, SOL_LWM2M_OBJECT_API_VERSION); \
            return __VA_ARGS__; \
        } \
    } while (0);
#define LWM2M_OBJECT_CHECK_API_GOTO(_obj, _label) \
    do { \
        if (SOL_UNLIKELY((_obj).api_version != \
            SOL_LWM2M_OBJECT_API_VERSION)) { \
            SOL_WRN("Couldn't handle object that has unsupported version " \
                "'%u', expected version is '%u'", \
                (_obj).api_version, SOL_LWM2M_OBJECT_API_VERSION); \
            goto _label; \
        } \
    } while (0);
#else
#define LWM2M_TLV_CHECK_API(_tlv, ...)
#define LWM2M_RESOURCE_CHECK_API(_resource, ...)
#define LWM2M_OBJECT_CHECK_API(_obj, ...)
#define LWM2M_RESOURCE_CHECK_API_GOTO(_resource, _label)
#define LWM2M_OBJECT_CHECK_API_GOTO(_obj, _label)
#endif

enum tlv_length_size_type {
    LENGTH_SIZE_CHECK_NEXT_TWO_BITS = 0,
    LENGTH_SIZE_8_BITS = 8,
    LENGTH_SIZE_16_BITS = 16,
    LENGTH_SIZE_24_BITS = 24
};

enum lwm2m_parser_args_state {
    STATE_NEEDS_DIGIT = 0,
    STATE_NEEDS_COMMA_OR_EQUAL = (1 << 1),
    STATE_NEEDS_COMMA = (1 << 2),
    STATE_NEEDS_APOSTROPHE = (1 << 3),
    STATE_NEEDS_CHAR_OR_DIGIT = (1 << 4),
};

struct lifetime_ctx {
    struct sol_timeout *timeout;
    uint32_t lifetime;
};

struct sol_lwm2m_client_object {
    struct sol_ptr_vector instances;
    uint16_t id;
};

void
send_ack_if_needed(struct sol_coap_server *coap, struct sol_coap_packet *msg,
    const struct sol_network_link_addr *cliaddr);

enum sol_lwm2m_binding_mode
get_binding_mode_from_str(const struct sol_str_slice binding);

void
client_objects_clear(struct sol_ptr_vector *objects);

struct sol_lwm2m_client_object *
find_client_object_by_id(struct sol_ptr_vector *objects,
    uint16_t id);

int
add_to_monitors(struct sol_monitors *monitors, sol_monitors_cb_t cb, const void *data);

int
remove_from_monitors(struct sol_monitors *monitors, sol_monitors_cb_t cb, const void *data);

size_t
get_int_size(int64_t i);

int
get_resource_len(const struct sol_lwm2m_resource *resource, uint16_t index,
    size_t *len);

void
swap_bytes(uint8_t *to_swap, size_t len);

int
add_float_resource(struct sol_buffer *buf, double fp, size_t len);

int
add_int_resource(struct sol_buffer *buf, int64_t i, size_t len);

int
add_resource_bytes_to_buffer(const struct sol_lwm2m_resource *resource,
    struct sol_buffer *buf, uint16_t idx);

int
set_packet_payload(struct sol_coap_packet *pkt,
    const uint8_t *data, uint16_t len);

int
setup_tlv_header(enum sol_lwm2m_tlv_type tlv_type, uint16_t res_id,
    struct sol_buffer *buf, size_t data_len);

int
setup_tlv(struct sol_lwm2m_resource *resource, struct sol_buffer *buf);

int
resources_to_tlv(struct sol_lwm2m_resource *resources,
    size_t len, struct sol_buffer *tlvs);

int
instances_to_tlv(struct sol_lwm2m_resource **instances,
    size_t *instances_len, uint16_t *instances_ids, size_t len, struct sol_buffer *tlvs);

int
add_coap_int_option(struct sol_coap_packet *pkt,
    enum sol_coap_option opt, const void *data, uint16_t len);

int
get_coap_int_option(struct sol_coap_packet *pkt,
    enum sol_coap_option opt, uint16_t *value);

int
setup_coap_packet(enum sol_coap_method method,
    enum sol_coap_message_type type, const char *objects_path, const char *path,
    uint8_t *obs, int64_t *token, struct sol_lwm2m_resource *resources,
    struct sol_lwm2m_resource **instances, size_t *instances_len,
    uint16_t *instances_ids, size_t len,
    const char *execute_args,
    struct sol_coap_packet **pkt);

void
tlv_clear(struct sol_lwm2m_tlv *tlv);

int
is_resource(struct sol_lwm2m_tlv *tlv);
