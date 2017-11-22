/*
 * Copyright (c) 2017, Annika Wierichs, RWTH Aachen University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the University nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * TODO: Documentation
 *
 */

#include "uhyve-ibv.h"

#include <infiniband/verbs.h>		// Linux include


/*
 * ibv_get_device_list
 */

void call_ibv_get_device_list(struct kvm_run * run, uint8_t * guest_mem) {
	printf("LOG: UHYVE - call_ibv_get_device_list\n");
	unsigned data = *((unsigned *)((size_t)run+run->io.data_offset));
	uhyve_ibv_get_device_list_t * args = (uhyve_ibv_get_device_list_t *) (guest_mem + data);

	// Call IBV function from hypervisor
	int num_devices;
	struct ibv_device **host_ret = ibv_get_device_list(&num_devices);

	// Copy number of devices and device stucts to kernel memory
	if (args->num_devices) {
		memcpy(args->num_devices, &num_devices, sizeof(num_devices));
	}
	for (int d = 0; d < num_devices; d++) {
		memcpy(args->ret[d], host_ret[d], sizeof(struct ibv_device));
	}
}


/*
 * ibv_get_device_name
 */

void call_ibv_get_device_name(struct kvm_run * run, uint8_t * guest_mem) {
	printf("LOG: UHYVE - call_ibv_get_device_name");
	unsigned data = *((unsigned*)((size_t)run+run->io.data_offset));
	uhyve_ibv_get_device_name_t * args = (uhyve_ibv_get_device_name_t *) (guest_mem + data);

	// TODO: Tricky because char ptr isn't allocated in called function.
	args->ret = ibv_get_device_name(args->device);
	/* memcpy(args->ret, host_ret, sizeof(host_ret)); */
	// TODO: Convert ptrs contained in return value.
	// TODO: How to tell if ret needs to be deleted?
}


/*
 * ibv_open_device
 */

void call_ibv_open_device(struct kvm_run * run, uint8_t * guest_mem) {
	printf("LOG: UHYVE - call_ibv_open_device\n");
	unsigned data = *((unsigned*)((size_t)run+run->io.data_offset));
	uhyve_ibv_open_device_t * args = (uhyve_ibv_open_device_t *) (guest_mem + data);

	printf("LOG: UHYVE - call_ibv_open_device\n");
	struct ibv_context * host_ret = ibv_open_device(args->device);
	printf("LOG: UHYVE - call_ibv_open_device\n");
	memcpy(args->ret, host_ret, sizeof(host_ret));
	printf("LOG: UHYVE - call_ibv_open_device\n");
	// TODO: Convert ptrs contained in return value.
	free(host_ret);
	printf("LOG: UHYVE - call_ibv_open_device\n");
}


/*
 * ibv_query_port
 */

void call_ibv_query_port(struct kvm_run * run, uint8_t * guest_mem) {
	printf("LOG: UHYVE - call_ibv_query_port");
	unsigned data = *((unsigned*)((size_t)run+run->io.data_offset));
	uhyve_ibv_query_port_t * args = (uhyve_ibv_query_port_t *) (guest_mem + data);

	int host_ret = ibv_query_port(args->context, args->port_num, args->port_attr);
	args->ret = host_ret;
}


/*
 * ibv_create_comp_channel
 */

void call_ibv_create_comp_channel(struct kvm_run * run, uint8_t * guest_mem) {
	printf("LOG: UHYVE - call_ibv_create_comp_channel");
	unsigned data = *((unsigned*)((size_t)run+run->io.data_offset));
	uhyve_ibv_create_comp_channel_t * args = (uhyve_ibv_create_comp_channel_t *) (guest_mem + data);
	/*uhyve_ibv_create_comp_channel_t * args = (uhyve_ibv_create_comp_channel_t *) get_data(run);*/

	struct ibv_comp_channel * host_ret = ibv_create_comp_channel(args->context);
	memcpy(args->ret, host_ret, sizeof(host_ret)); // TODO: This will only work for ABI ver > 2.
	free(host_ret);
}

