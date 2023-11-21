//
// Created by zyn66 on 11/18/2023.
//

#include <stdio.h>
#include <stdbool.h>
#include "protocal.h"

void wait_for_event(event_type *event){
    printf("Waiting for event");

    // Loop to print dots every second for a total of 3 seconds
    for (int i = 0; i < 3; ++i) {
        fflush(stdout);  // Flush the output buffer to ensure immediate printing
        printf(".");
    }
    printf("\n");
}

void is_received(bool status){
    status ? printf("Data Received\n") : printf("Data not received\n");
}

void from_network_layer(packet p){
    printf("Take Packet %s from network layer to physical layer\n",p);
}

void to_network_layer(packet p){
    printf("Take Packet %s from physical layer to network layer\n",p);
}


void from_physical_layer(frame r){
    printf("Take frame %r to network layer\n",r.info);
}


void to_physical_layer(frame s){
    printf("Take frame %r to network layer\n",s.info);
}


void start_timer(seq_nr k){
    printf("Start Timer for sequence number %i\n",k);
    timer = true;
}


void stop_timer(seq_nr k){
    printf("End Timer for sequence number %i\n",k);
    timer = false;
}


void start_ack_timer(void){
    printf("Start Acknowledge Time for sequence number\n");
    timer = true;
}


void stop_ack_timer(void){
    printf("End Acknowledge Time for sequence number\n");
    timer = true;
}


void enable_network_layer(void){
    printf("Enable Network layer\n");
}




void disable_network_layer(void){
    printf("Disable Network layer\n");
}


void displayEvent(event_type event) {
    switch (event) {
        case frame_arrival:
            printf("Event: Frame Arrival\n");
            break;
        case cksum_err:
            printf("Event: Checksum Error\n");
            break;
        case time_out:
            printf("Event: Timeout\n");
            break;
        case network_layer_ready:
            printf("Event: Network Layer Ready\n");
            break;
        case ready:
            printf("Event: Ready\n");
            break;
        default:
            printf("Unknown Event\n");
    }
}

