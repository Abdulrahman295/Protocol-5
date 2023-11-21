#include <stdio.h>
#include <stdbool.h>
#include "protocol5/protocol5.h"
#include "protocol/protocal.h"
int main() {
    char Data[]="Hello, this is protocol 5";
    // test protocal.c functions:
    // seq_nr n = 5;

    // packet p;
    // p.data[0] = Data[0];
    // printf("%c\n", p.data[0]);

    // frame_kind fk;
    // fk = ack;
    // printf("%d\n", fk);

    // frame f;
    // f.kind = fk;
    // f.seq = 5;
    // f.ack = 4;
    // f.info = p;
    // printf("%d -- %d -- %d -- %c\n", f.kind, f.seq, f.ack, f.info.data[0]);


    // event_type e;
    // e = time_out;
    // printf("event_type %d\n", e);


    // frame_arrives fa;
    // fa = arrive;
    // printf("frame_arrives %d\n", fa);

    int k = 3; 
    // inc(k);
    // printf("increament %d\n", k);


    // // int x;
    // // printf("Enter an integer: ");
    // // scanf("\n%d", &x);
    // // printf("You entered: %d ", x);


    // ////////////
    // wait_for_event(&e);
    // is_received(true);
    // start_timer(n);

    // test protocol5
    packet SenderPackets[10],receivedPackets[10];
    frame SenderFrames[10],receivedFrames[10];
    int packets_length = Serialize_to_packet(Data,SenderPackets);
    // int frames_length = Serialize_to_frame(SenderPackets, SenderFrames, packets_length);
    display_packets(SenderPackets, packets_length);
    // display_frames(SenderFrames, frames_length);
    // printf("after000\n");
    // printf("fisrt char: %s", SenderPackets[0].data);
    return 0;
}
