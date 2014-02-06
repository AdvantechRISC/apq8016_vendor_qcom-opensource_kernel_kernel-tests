/*
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of The Linux Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <linux/if_ether.h>

#include "Constants.h"
#include "Logger.h"
#include "linux/msm_ipa.h"
#include "TestsUtils.h"
#include "TestBase.h"
#include "Pipe.h"
#include "RoutingDriverWrapper.h"
#include "HeaderInsertion.h"
#include "Filtering.h"
#include "IPAFilteringTable.h"
#define MAX_PACKET_SIZE 1024


#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
struct RndisHeader {
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t DataOffset;
	uint32_t DataLength;
	uint32_t OOBDataOffset;
	uint32_t OOBDataLength;
	uint32_t OOBNumber;
	uint32_t PacketInfoOffset;
	uint32_t PacketInfoLength;
	uint64_t Reserved;
};

struct RndisEtherHeader {
	struct RndisHeader rndisHeader;
	struct ethhdr etherHeader;
};
#pragma pack(pop)   /* restore original alignment from stack */

/*This class will be the base class of RNDIS Aggregation tests.
 *Any method other than the test case itself can be
 *declared in this Fixture thus allowing the derived classes to
 *implement only the test case.
 *All the test of the Aggregation uses one input and one output in DMA mode.
 */
class RNDISAggregationTestFixture:public TestBase
{
public:
	/*This Constructor will register each instance that it creates.*/
	RNDISAggregationTestFixture();

	/*This method will create and initialize two Pipe object for the USB
	 *(Ethernet) Pipes, one as input and the other as output.
	 */
	virtual bool Setup();

	/*This method will destroy the pipes.*/
	virtual bool Teardown();

	virtual bool Run();

	virtual bool AddRules() = 0;

	virtual bool TestLogic() = 0;

	bool AddRulesNoAgg();

	bool AddRulesDeAggEther();

	bool AddRulesAggTimeLimit();

	bool AddRulesAggByteLimit();

	bool AddRulesAggPacketLimit();

	/*The client type are set from the peripheral perspective*/
	static Pipe m_IpaToUsbPipeAgg;
	/*IPA_CLIENT_USB2_CONS (pipe_num = 7)*/
	static Pipe m_UsbToIpaPipe;
	/* IPA_CLIENT_USB_PROD (pipe_num = 11)*/
	static Pipe m_IpaToUsbPipe;
	/* IPA_CLIENT_USB3_CONS (pipe_num = 9)*/
	static Pipe m_UsbToIpaPipeDeagg;
	/* IPA_CLIENT_USB2_PROD (pipe_num = 6)*/
	static Pipe m_IpaToUsbPipeAggTime;
	/* IPA_CLIENT_USB_CONS (pipe_num = 10)*/
	static Pipe m_IpaToUsbPipeAggPktLimit;
	/* IPA_CLIENT_USB4_CONS (pipe_num = 4)*/
	static Pipe m_HsicToIpaPipe;
	/* IPA_CLIENT_Q6_LAN_PROD (pipe_num = 19)*/

	static RoutingDriverWrapper m_Routing;
	static Filtering m_Filtering;
	static HeaderInsertion m_HeaderInsertion;

	static const Byte m_EtherHeader[14];

protected:
	enum ipa_ip_type m_eIP;
};