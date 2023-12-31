//
// Created by zyn66 on 11/19/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../protocol/protocal.h"
#include "protocol5.h"
/* Protocol 5 (Go-back-n) allows multiple outstanding frames. The sender may transmit up
to MAX SEQ frames without waiting for an ack. In addition, unlike in the previous
protocols, the network layer is not assumed to have a new packet all the time. Instead,
the network layer causes a network layer ready event when there is a packet to send. */

int Serialize_to_packet(char *sentence, packet *words) {
    // Create a temporary copy of the sentence to avoid modifying the original string
    char sentenceCopy[1000];
    strcpy(sentenceCopy, sentence);

    // Use strtok to tokenize the sentence based on space characters
    char *token = strtok(sentenceCopy, " ");
    int i = 0;

    // Loop through the tokens and copy them into the words array
    while (token != NULL) {
        strcpy(words[i].data, token);
        token = strtok(NULL, " ");
        i++;
    }

    // return length of current packets
    return i;
}

int Serialize_to_frame(packet *words, frame *frames, int packets_len) {
    int i = 0;

    // Loop through the words and copy them into the frames array
    while (i < packets_len) {
        frames[i].kind = data;   // Assuming all are data frames
        frames[i].seq = i;             // Sequence number
        frames[i].ack = 0;             // Acknowledgement number (assuming it's not used for data frames)
        strcpy(frames[i].info.data, words[i].data);  // Copy the packet data into the frame
        i++;
    }

    // Ensure the last entry in the frames array is empty
    frames[i].kind = data;
    frames[i].seq = i;
    frames[i].ack = 0;
    strcpy(frames[i].info.data,'\0');

    //return length of current frames
    return i+1;
}

void display_frames(const frame *frames, int len) {
    printf("Frames of size: %d\n", len);
    for (int i = 0; i < len; i++) {
        printf("Frame %d:\n", i + 1);
        printf("  Kind: %d\n", frames[i].kind);
        printf("  Seq: %d\n", frames[i].seq);
        printf("  Ack: %d\n", frames[i].ack);
        printf("  Data: %s\n", frames[i].info.data);
        printf("\n");
    }
}

void display_packets(const packet *words, int len) {
    printf("Packets of size: %d\n", len);
    for (int i = 0; i < len; ++i) {
        printf("Packet %d: %s\n", i + 1, words[i].data);
    }
}


static bool between(seq_nr a, seq_nr b, seq_nr c)
{
/* Return true if a <= b < c circularly; false otherwise. */
    if (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
        return(true);
    else
        return(false);
}


static void Receive_data(frame s , frame frames[],seq_nr frame_nr){
    /*to randomize the sequence of sending Frames */
    frame_arrives x  = (frame_arrives)(rand() % 3);
    if(x == arrive){
        frames[frame_nr] = s;
    }
    else if(x == Cksum_err || x == Frame_missed){
        s.ack = -1;
        frames[frame_nr] = s;
    }
}

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[], frame Receiver[])
{
    /* Construct and send a data frame. */
    frame s; /* scratch variable */
    s.info = buffer[frame_nr]; /* insert packet into frame */
    s.seq = frame_nr; /* insert sequence number into frame */
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);/* piggyback ack */
    to_physical_layer(s); /* transmit the frame */
    Receive_data( s,Receiver,frame_nr);
    start_timer(frame_nr); /* start the timer running */
}


bool is_frame_expected(frame receiver[],seq_nr frame_expected) {
    for (int i = 0; i <= MAX_SEQ; i++) {
        if (receiver[i].seq == frame_expected) {
            return true;
        }
    }
    return false;
}
event_type getRandomEvent() {
    return (event_type)(rand() % 5);
}

// void protocol5(const char *sentence)
// {
//     packet SenderPackets[MAX_SEQ],receivedPackets[MAX_SEQ];
//     frame SenderFrames[MAX_SEQ],receivedFrames[MAX_SEQ];
//     Serialize_to_packet(sentence,SenderPackets);
//     Serialize_to_frame(SenderPackets,SenderFrames);
//     display_packets(SenderPackets);
//     display_frames(SenderFrames);

//     seq_nr next_frame_to_send = 0; /* MAX SEQ > 1; used for outbound stream */
//     seq_nr ack_expected = 0; /* oldest frame as yet unacknowledged */
//     seq_nr frame_expected = 0; /* next frame expected on inbound stream */
//     frame r; /* scratch variable */
//     seq_nr nbuffered =0; /* number of output buffers currently in use */
//     seq_nr i; /* used to index into the buffer array */
//     event_type event;
//     enable_network_layer(); /* allow network layer ready events */
//     //boolean sender_Or_receiver = true; /* Set sender = true , receiver = false */
//     // first send data from receiver and get in while loop until send every frames
//     from_network_layer(SenderPackets[next_frame_to_send]); /* fetch new packet */
//     nbuffered = nbuffered + 1; /* expand the sender’s window */
//     send_data(next_frame_to_send, frame_expected, SenderPackets,receivedFrames);/* transmit the frame */
//     inc(next_frame_to_send); /* advance sender’s upper window edge */
//     // begin Task
//     while (true) {
//         wait_for_event(&event);/* four possibilities: see event type above */
//         displayEvent(event);
//         switch(event) {
//             case network_layer_ready: /* the network layer has a packet to send */
//                 /* Accept, save, and transmit a new frame. */
//                 from_network_layer(SenderPackets[next_frame_to_send]); /* fetch new packet */
//                 nbuffered = nbuffered + 1; /* expand the sender’s window */
//                 send_data(next_frame_to_send, frame_expected, SenderPackets,receivedFrames);/* transmit the frame */
//                 inc(next_frame_to_send); /* advance sender’s upper window edge */
//                 break;
//             case frame_arrival: /* a data or control frame has arrived */
//                 from_physical_layer(SenderFrames[next_frame_to_send-1]); /* get incoming frame from physical layer */
//                 if (is_frame_expected(receivedFrames, frame_expected)) {
//                 /* Frames are accepted only in order. */
//                     to_network_layer(receivedFrames[frame_expected].info); /* pass packet to network layer */
//                     inc(frame_expected); /* advance lower edge of receiver’s window */
//                 }
//                 /* Ack n implies n − 1, n − 2, etc. Check for this. */
//                 while (between(ack_expected, r .ack, next_frame_to_send)) {
//                     /* Handle piggybacked ack. */
//                     nbuffered--; /* one frame fewer buffered */
//                     stop_timer(ack_expected); /* frame arrived intact; stop timer */
//                     inc(ack_expected); /* contract sender’s window */
//                 }
//                 break;

//             case cksum_err: break; /* just ignore bad frames */

//             case time_out: /* trouble; retransmit all outstanding frames */
//                 next_frame_to_send = ack_expected; /* start retransmitting here */
//                 for (i = 1; i <= nbuffered; i++) {
//                     send_data(next_frame_to_send, frame_expected, SenderPackets,receivedFrames);/* resend frame */
//                     inc(next_frame_to_send); /* prepare to send the next one */
//                 }
//         }
//       //  sender_Or_receiver = ~ sender_Or_receiver;
//         if (nbuffered < MAX_SEQ)
//         enable_network_layer();
//         else
//         disable_network_layer();
//     }
// }
