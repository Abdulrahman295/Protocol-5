//
// Created by zyn66 on 11/19/2023.
//
#include <stdbool.h>
#include "../protocol/protocal.h"
#define MAX_SEQ 7

int Serialize_to_packet(char *sentence, packet *words);

int Serialize_to_frame(packet *words, frame *frames,  int packets_len);

void display_frames(const frame *frames, int len);

void display_packets(const packet *words, int lent);

static bool between(seq_nr a, seq_nr b, seq_nr c);

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[], frame Receiver[]);

void protocol5(const char *sentence);

event_type getRandomEvent();

bool is_frame_expected(frame receiver[],seq_nr frame_expected);