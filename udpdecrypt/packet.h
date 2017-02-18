#include "public.h"
#pragma once

// Define the structure of game packets
#ifndef PACKET_H
#define PACKET_H
struct packet {
	// Header is 24 bytes
	// Length of the UDP data
	uint16_t length;
	// Optional flags?
	uint16_t flags;
	// CRC32 hash
	uint32_t crc32;
	// SEQ number - Gets incremented every time a packet is sent?
	uint32_t serial;
	// These fields have to do with the transport of packets
	uint32_t origin;
	uint32_t control1;
	uint32_t control2;

	// data starts at length - 24 bytes
	unsigned char* data;
};
#endif

// NOTE: These are the old arma 2 flags, they could be wrong.
enum PacketFlags : uint16_t
{
	VIM_FLAG = 0x8000,
	/// Urgent message flag.
	URGENT_FLAG = 0x4000,
	ORDERED_FLAG = 0x2000,
	FROM_BCAST_FLAG = 0x1000,
	TO_BCAST_FLAG = 0x0800,
	DELAY_FLAG = 0x0400,
	INSTANT_FLAG = 0x0200,
	BANDWIDTH_FLAG = 0x0100,
	BUNCH_FLAG = 0x0080,
	LAYER_FLAG = 0x0040,
	PART_FLAG = 0x0020,
	CLOSING_FLAG = 0x0010,
	VOICE_FLAG = 0x0008,
	USER_FLAGS = 0x0007,
	SPECIAL_FLAG = 0x0001,
	ALL_FLAGS = 0xffff
};

enum Offsets : uint16_t
{
	HEADER_SIZE = 24
};

/*
I think a network type of 3 indicates a network message which contains many network messages to be received in one packet. 
A type of 12 appears to be player chat and a type of 10 indicates public variable information. 
There might be a prefix'd length or further decoding required that is throwing me off.
*/