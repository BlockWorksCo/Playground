


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "smallz4.h"


//char    inputData[]             = "Hello World Hello World Hello World.";
char    inputData[]             = " \
2019-02-10  8:45:23.675 #1   0000000000000001 <log> 0:00:00.754 [debug] [ meter/assoc] {   device}: Disconnected from low-level association. \
2019-02-10  8:45:23.675 #1   0000000000000001 <log> 0:00:00.754 [debug] [ meter/assoc] {   device}: Send IC read request for 2 \
2019-02-10  8:45:23.675 #1   0000000000000001 <log> 0:00:00.754 [debug] [        dlms] {   device}: Get 0-0:43.1.3*255 ic 1 attr 2 normal access \
2019-02-10  8:45:23.726 #1   0000000000000001 <log> 0:00:00.804 [debug] [        hdlc] {   device}: tx: (1/1): 7e \
2019-02-10  8:45:23.726 #1   0000000000000001 <log> 0:00:00.805 [debug] [        hdlc] {   device}: tx: (1/1): a019 \
2019-02-10  8:45:23.726 #1   0000000000000001 <log> 0:00:00.805 [debug] [        hdlc] {   device}: tx: (1/1): 03 \
2019-02-10  8:45:23.726 #1   0000000000000001 <log> 0:00:00.805 [debug] [        hdlc] {   device}: tx: (1/1): 21 \
2019-02-10  8:45:23.726 #1   0000000000000001 <log> 0:00:00.805 [debug] [        hdlc] {   device}: tx: (1/1): 32 \
2019-02-10  8:45:23.726 #1   0000000000000001 <log> 0:00:00.805 [debug] [        hdlc] {   device}: tx: (1/1): 6fd8 \
2019-02-10  8:45:23.726 #1   0000000000000001 <log> 0:00:00.805 [debug] [        hdlc] {   device}: tx: (1/1): e6e600 \
2019-02-10  8:45:23.726 #1   0000000000000001 <log> 0:00:00.805 [debug] [        hdlc] {   device}: tx: (1/1): c00181000100002b0103ff0200 \
2019-02-10  8:45:23.727 #1   0000000000000001 <log> 0:00:00.805 [debug] [        hdlc] {   device}: tx: (1/1): b567 \
2019-02-10  8:45:23.727 #1   0000000000000001 <log> 0:00:00.805 [debug] [        hdlc] {   device}: tx: (1/1): 7e \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.865 [debug] [        hdlc] {     hdlc}: ** giveCause = 3, 1 ** \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.865 [debug] [        hdlc] {     hdlc}: rx: (1/2): 7ea0152103525d8ae6e700c401810006 \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.865 [debug] [        hdlc] {     hdlc}: rx: (2/2): 003ce488cf7a7e \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.865 [debug] [        hdlc] {     hdlc}: ** Waiting for HDLC event (60000ms) ** \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.865 [debug] [        dlms] {     dlms}: DLMS result is 5 bytes with code 0 \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.865 [debug] [meter/dlmscomms] {     dlms}: IN) dlmsReceiveCb called with eventType ResponseReceived \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.865 [debug] [meter/dlmscomms] {     dlms}:  0) ResponseReceived  \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.866 [debug] [meter/dlmscomms] {     dlms}:  ** 1 messages in DLMS queue **  \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.866 [debug] [meter/dlmscomms] {   device}: USE) releasing eventType ResponseReceived \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.866 [debug] [meter/dlmscomms] {   device}: Got response from meter (eventType ResponseReceived). \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.866 [debug] [meter/dlmscomms] {   device}:  0) ResponseReceived  \
2019-02-10  8:45:23.787 #1   0000000000000001 <log> 0:00:00.866 [debug] [meter/dlmscomms] {   device}:  ** 1 messages in DLMS queue **  \
2019-02-10  8:45:23.837 #1   0000000000000001 <log> 0:00:00.916 [debug] [        dlms] {   device}: Disconnect HDLC link \
2019-02-10  8:45:23.837 #1   0000000000000001 <log> 0:00:00.916 [debug] [        hdlc] {   device}: tx: (1/1): 7e \
2019-02-10  8:45:23.838 #1   0000000000000001 <log> 0:00:00.916 [debug] [        hdlc] {   device}: tx: (1/1): a007 \
2019-02-10  8:45:23.838 #1   0000000000000001 <log> 0:00:00.916 [debug] [        hdlc] {   device}: tx: (1/1): 03 \
2019-02-10  8:45:23.838 #1   0000000000000001 <log> 0:00:00.916 [debug] [        hdlc] {   device}: tx: (1/1): 21 \
2019-02-10  8:45:23.838 #1   0000000000000001 <log> 0:00:00.916 [debug] [        hdlc] {   device}: tx: (1/1): 53 \
2019-02-10  8:45:23.838 #1   0000000000000001 <log> 0:00:00.916 [debug] [        hdlc] {   device}: tx: (1/1): 03c7 \
2019-02-10  8:45:23.838 #1   0000000000000001 <log> 0:00:00.916 [debug] [        hdlc] {   device}: tx: (1/1): 7e \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.961 [debug] [        hdlc] {     hdlc}: ** giveCause = 3, 1 ** \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.961 [debug] [        hdlc] {     hdlc}: rx: (1/2): 7ea01e210373c37a8180120501800601 \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.961 [debug] [        hdlc] {     hdlc}: rx: (2/2): 80070400000001080400000001533b7e \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.961 [debug] [        hdlc] {     hdlc}: HDLC link disconnected \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [        dlms] {     dlms}: DLMS result is 0 bytes with code 12 \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {     dlms}: IN) dlmsReceiveCb called with eventType Disconnected \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {     dlms}:  0) ResponseReceived  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {     dlms}:  1) Disconnected  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {     dlms}:  ** 2 messages in DLMS queue **  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [        dlms] {     dlms}: DLMS result is 0 bytes with code 0 \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {     dlms}: IN) dlmsReceiveCb called with eventType Disconnected2 \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {     dlms}:  0) ResponseReceived  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {     dlms}:  1) Disconnected  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {     dlms}:  2) Disconnected2  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {     dlms}:  ** 3 messages in DLMS queue **  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [        hdlc] {     hdlc}: ** Waiting for HDLC event (60000ms) ** \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}: USE) releasing eventType Disconnected \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}: Got response from meter (eventType Disconnected). \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  0) ResponseReceived  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  1) Disconnected  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  2) Disconnected2  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  ** 3 messages in DLMS queue **  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}: OUT) releasing eventType Disconnected \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}: Releasing a response! \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  0) ResponseReceived  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  2) Disconnected2  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  ** 2 messages in DLMS queue **  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [ meter/assoc] {   device}: ** Got assoc response 0 ** \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}: USE) releasing eventType Disconnected2 \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}: Got response from meter (eventType Disconnected2). \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  0) ResponseReceived  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  2) Disconnected2  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  ** 2 messages in DLMS queue **  \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}: OUT) releasing eventType Disconnected2 \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}: Releasing a response! \
2019-02-10  8:45:23.883 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  0) ResponseReceived  \
2019-02-10  8:45:23.884 #1   0000000000000001 <log> 0:00:00.962 [debug] [meter/dlmscomms] {   device}:  ** 1 messages in DLMS queue **  \
2019-02-10  8:45:23.884 #1   0000000000000001 <log> 0:00:00.962 [debug] [ meter/assoc] {   device}: ** Got assoc response 1 ** \
2019-02-10  8:45:23.884 #1   0000000000000001 <log> 0:00:00.962 [debug] [        hdlc] {   device}: disconnectLink 0x0 \
2019-02-10  8:45:23.884 #1   0000000000000001 <log> 0:00:00.962 [debug] [        dlms] {   device}: frameCounter:3991611 \
2019-02-10  8:45:23.884 #1   0000000000000001 <log> 0:00:00.962 [debug] [        dlms] {   device}: Setup AA with HLS \
2019-02-10  8:45:23.934 #1   0000000000000001 <log> 0:00:01.012 [debug] [        hdlc] {   device}: Connect HDLC link \
2019-02-10  8:45:23.934 #1   0000000000000001 <log> 0:00:01.012 [debug] [        hdlc] {   device}: tx: (1/1): 7e \
2019-02-10  8:45:23.935 #1   0000000000000001 <log> 0:00:01.012 [debug] [        hdlc] {   device}: tx: (1/1): a020 \
2019-02-10  8:45:23.935 #1   0000000000000001 <log> 0:00:01.012 [debug] [        hdlc] {   device}: tx: (1/1): 03 \
2019-02-10  8:45:23.935 #1   0000000000000001 <log> 0:00:01.013 [debug] [        hdlc] {   device}: tx: (1/1): 61 \
2019-02-10  8:45:23.935 #1   0000000000000001 <log> 0:00:01.013 [debug] [        hdlc] {   device}: tx: (1/1): 93 \
2019-02-10  8:45:23.935 #1   0000000000000001 <log> 0:00:01.013 [debug] [        hdlc] {   device}: tx: (1/1): 1b9f \
2019-02-10  8:45:23.936 #1   0000000000000001 <log> 0:00:01.013 [debug] [        hdlc] {   device}: tx: (1/2): 81801405020080060200800704000000 \
2019-02-10  8:45:23.936 #1   0000000000000001 <log> 0:00:01.013 [debug] [        hdlc] {   device}: tx: (2/2): 01080400000001 \
2019-02-10  8:45:23.936 #1   0000000000000001 <log> 0:00:01.014 [debug] [        hdlc] {   device}: tx: (1/1): ce6a \
2019-02-10  8:45:23.936 #1   0000000000000001 <log> 0:00:01.014 [debug] [        hdlc] {   device}: tx: (1/1): 7e \
2019-02-10  8:45:24.010 #1   0000000000000001 <log> 0:00:01.088 [debug] [        hdlc] {     hdlc}: ** giveCause = 3, 1 ** \
2019-02-10  8:45:24.010 #1   0000000000000001 <log> 0:00:01.088 [debug] [        hdlc] {     hdlc}: rx: (1/2): 7ea01e610373b57c8180120501800601 \
2019-02-10  8:45:24.010 #1   0000000000000001 <log> 0:00:01.088 [debug] [        hdlc] {     hdlc}: rx: (2/2): 80070400000001080400000001533b7e \
2019-02-10  8:45:24.010 #1   0000000000000001 <log> 0:00:01.088 [debug] [        hdlc] {     hdlc}: tx: (1/1): 7e \
2019-02-10  8:45:24.010 #1   0000000000000001 <log> 0:00:01.088 [debug] [        hdlc] {     hdlc}: tx: (1/1): a07c \
2019-02-10  8:45:24.010 #1   0000000000000001 <log> 0:00:01.088 [debug] [        hdlc] {     hdlc}: tx: (1/1): 03 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.088 [debug] [        hdlc] {     hdlc}: tx: (1/1): 61 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.088 [debug] [        hdlc] {     hdlc}: tx: (1/1): 10 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (1/1): aa6b \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (1/1): e6e600 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (1/7): 606ea109060760857405080103a60a04 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (2/7): 084c54434c49454e548a0207808b0760 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (3/7): 857405080202ac128010306545374563 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (4/7): 63633938356635344342be3404322130 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (5/7): 30003ce83bbbf09a779d1a0bb2321b56 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (6/7): 8112bd077230d19b46d21d43c1c61289 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (7/7): 3f62f25cd6c7e27d9e88336f048006dd \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (1/1): 3e06 \
2019-02-10  8:45:24.011 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: tx: (1/1): 7e \
2019-02-10  8:45:24.012 #1   0000000000000001 <log> 0:00:01.089 [debug] [        hdlc] {     hdlc}: ** Waiting for HDLC event (60000ms) ** \
2019-02-10  8:45:24.298 #1   0000000000000001 <log> 0:00:01.376 [debug] [        hdlc] {     hdlc}: ** giveCause = 3, 1 ** \
2019-02-10  8:45:24.298 #1   0000000000000001 <log> 0:00:01.376 [debug] [        hdlc] {     hdlc}: rx: (1/8): 7ea077610330ad6ce6e7006169a10906 \
2019-02-10  8:45:24.299 #1   0000000000000001 <log> 0:00:01.376 [debug] [        hdlc] {     hdlc}: rx: (2/8): 0760857405080103a203020100a305a1 \
2019-02-10  8:45:24.299 #1   0000000000000001 <log> 0:00:01.377 [debug] [        hdlc] {     hdlc}: rx: (3/8): 0302010ea40a04084c4e543031313830 \
2019-02-10  8:45:24.299 #1   0000000000000001 <log> 0:00:01.377 [debug] [        hdlc] {     hdlc}: rx: (4/8): 88020780890760857405080202aa1280 \
2019-02-10  8:45:24.299 #1   0000000000000001 <log> 0:00:01.377 [debug] [        hdlc] {     hdlc}: rx: (5/8): 106162636465666768696a6b6c6d6e6f \
2019-02-10  8:45:24.299 #1   0000000000000001 <log> 0:00:01.377 [debug] [        hdlc] {     hdlc}: rx: (6/8): 70be230421281f30000094338ee4f259 \
2019-02-10  8:45:24.299 #1   0000000000000001 <log> 0:00:01.377 [debug] [        hdlc] {     hdlc}: rx: (7/8): 04c8f470d8f333b73efc7a0dda3417d0 \
2019-02-10  8:45:24.300 #1   0000000000000001 <log> 0:00:01.377 [debug] [        hdlc] {     hdlc}: rx: (8/8): 712a7c8ccfc7cdbb7e \
2019-02-10  8:45:24.301 #1   0000000000000001 <log> 0:00:01.378 [debug] [        hdlc] {     hdlc}: ** Waiting for HDLC event (60000ms) ** \
2019-02-10  8:45:24.301 #1   0000000000000001 <log> 0:00:01.378 [debug] [        dlms] {     dlms}: rx decrypted: (1/1): 0800065f1f040000181d02300007 \
2019-02-10  8:45:24.301 #1   0000000000000001 <log> 0:00:01.378 [debug] [        dlms] {     dlms}: extracted server max pdu size is 1024 \
2019-02-10  8:45:24.301 #1   0000000000000001 <log> 0:00:01.378 [debug] [        dlms] {     dlms}: Action 0-0:40.0.0*255 ic 15 methodId 1 actionLen: 18 B \
2019-02-10  8:45:24.301 #1   0000000000000001 <log> 0:00:01.378 [debug] [        dlms] {     dlms}: tx unencrypted: (1/2): c30181000f0000280000ff01010910aa \
2019-02-10  8:45:24.301 #1   0000000000000001 <log> 0:00:01.378 [debug] [        dlms] {     dlms}: tx unencrypted: (2/2): 285231c6a65ee7fc5428be168dbc69 \
2019-02-10  8:45:24.353 #1   0000000000000001 <log> 0:00:01.429 [debug] [        hdlc] {     dlms}: tx: (1/1): 7e \
2019-02-10  8:45:24.354 #1   0000000000000001 <log> 0:00:01.430 [debug] [        hdlc] {     dlms}: tx: (1/1): a03e \
2019-02-10  8:45:24.354 #1   0000000000000001 <log> 0:00:01.430 [debug] [        hdlc] {     dlms}: tx: (1/1): 03 \
2019-02-10  8:45:24.354 #1   0000000000000001 <log> 0:00:01.430 [debug] [        hdlc] {     dlms}: tx: (1/1): 61 \
2019-02-10  8:45:24.354 #1   0000000000000001 <log> 0:00:01.430 [debug] [        hdlc] {     dlms}: tx: (1/1): 32 \
2019-02-10  8:45:24.355 #1   0000000000000001 <log> 0:00:01.430 [debug] [        hdlc] {     dlms}: tx: (1/1): 7b46 \
2019-02-10  8:45:24.355 #1   0000000000000001 <log> 0:00:01.431 [debug] [        hdlc] {     dlms}: tx: (1/1): e6e600 \
2019-02-10  8:45:24.355 #1   0000000000000001 <log> 0:00:01.431 [debug] [        hdlc] {     dlms}: tx: (1/4): cb3030003ce83cceaeb5f37a0e1737b3 \
2019-02-10  8:45:24.355 #1   0000000000000001 <log> 0:00:01.431 [debug] [        hdlc] {     dlms}: tx: (2/4): 02d201c7b3bec70058594b943fee5014 \
2019-02-10  8:45:24.355 #1   0000000000000001 <log> 0:00:01.431 [debug] [        hdlc] {     dlms}: tx: (3/4): 894eddecd9ccce0ae09a05ae979b5f31 \
2019-02-10  8:45:24.355 #1   0000000000000001 <log> 0:00:01.431 [debug] [        hdlc] {     dlms}: tx: (4/4): 12be \
2019-02-10  8:45:24.355 #1   0000000000000001 <log> 0:00:01.431 [debug] [        hdlc] {     dlms}: tx: (1/1): 284d \
2019-02-10  8:45:24.355 #1   0000000000000001 <log> 0:00:01.432 [debug] [        hdlc] {     dlms}: tx: (1/1): 7e \
2019-02-10  8:45:24.506 #1   0000000000000001 <log> 0:00:01.583 [debug] [        hdlc] {     hdlc}: ** giveCause = 3, 1 ** \
2019-02-10  8:45:24.506 #1   0000000000000001 <log> 0:00:01.584 [debug] [        hdlc] {     hdlc}: rx: (1/4): 7ea0376103520e3ae6e700cf29300000 \
2019-02-10  8:45:24.506 #1   0000000000000001 <log> 0:00:01.584 [debug] [        hdlc] {     hdlc}: rx: (2/4): 9434d33c4a83adea4a82889056a14865 \
2019-02-10  8:45:24.506 #1   0000000000000001 <log> 0:00:01.584 [debug] [        hdlc] {     hdlc}: rx: (3/4): c67759630b91a60e2768d0a41718c05b \
2019-02-10  8:45:24.506 #1   0000000000000001 <log> 0:00:01.584 [debug] [        hdlc] {     hdlc}: rx: (4/4): 63dc8082efd3a9d27e \
2019-02-10  8:45:24.506 #1   0000000000000001 <log> 0:00:01.584 [debug] [        hdlc] {     hdlc}: ** Waiting for HDLC event (60000ms) ** \
2019-02-10  8:45:24.506 #1   0000000000000001 <log> 0:00:01.584 [debug] [        dlms] {     dlms}: Encrypted:: (1/3): 293000009434d33c4a83adea4a828890 \
2019-02-10  8:45:24.506 #1   0000000000000001 <log> 0:00:01.584 [debug] [        dlms] {     dlms}: Encrypted:: (2/3): 56a14865c67759630b91a60e2768d0a4 \
2019-02-10  8:45:24.506 #1   0000000000000001 <log> 0:00:01.584 [debug] [        dlms] {     dlms}: Encrypted:: (3/3): 1718c05b63dc8082efd3 \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [        dlms] {     dlms}: rx decrypted: (1/2): c701810001000910c0bba41b053b9b78 \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [        dlms] {     dlms}: rx decrypted: (2/2): 4cec183f552e8ad6 \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [        dlms] {     dlms}: COSEM AA with HLS setup is accepted \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [        dlms] {     dlms}: DLMS result is 0 bytes with code 0 \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [meter/dlmscomms] {     dlms}: IN) dlmsReceiveCb called with eventType AssociationSuccessful \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [meter/dlmscomms] {     dlms}:  0) ResponseReceived  \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [meter/dlmscomms] {     dlms}:  1) AssociationSuccessful  \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [meter/dlmscomms] {     dlms}:  ** 2 messages in DLMS queue **  \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [meter/dlmscomms] {   device}: USE) releasing eventType AssociationSuccessful \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.584 [debug] [meter/dlmscomms] {   device}: Got response from meter (eventType AssociationSuccessful). \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.585 [debug] [meter/dlmscomms] {   device}:  0) ResponseReceived  \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.585 [debug] [meter/dlmscomms] {   device}:  1) AssociationSuccessful  \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.585 [debug] [meter/dlmscomms] {   device}:  ** 2 messages in DLMS queue **  \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.585 [debug] [meter/dlmscomms] {   device}: OUT) releasing eventType AssociationSuccessful \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.585 [debug] [meter/dlmscomms] {   device}: Releasing a response! \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.585 [debug] [meter/dlmscomms] {   device}:  0) ResponseReceived  \
2019-02-10  8:45:24.507 #1   0000000000000001 <log> 0:00:01.585 [debug] [meter/dlmscomms] {   device}:  ** 1 messages in DLMS queue **  \
2019-02-10  8:45:24.508 #1   0000000000000001 <log> 0:00:01.585 [debug] [ meter/assoc] {   device}: ** Associated ** \
2019-02-10  8:45:24.508 #1   0000000000000001 <log> 0:00:01.585 [debug] [meter/profile-discovery] {   devGetting profile metadata for Instantaneous \
2019-02-10  8:45:24.508 #1   0000000000000001 <log> 0:00:01.585 [debug] [        dlms] {   device}: Get 1-0:94.91.0*255 ic 7 attr 3 normal access \
";
char    outputData[64*1024]         = {0};
char    decompData[64*1024]         = {0};

char*   input                   = NULL;
size_t  inputBytesLeft          = 0;
char*   output                  = NULL;
size_t  outputSpaceAvailable    = 0;


// read one byte from input, see getByteFromIn()  for a basic implementation
typedef unsigned char (*decompGET_BYTE)  ();
// write several bytes,      see sendBytesToOut() for a basic implementation
typedef void          (*decompSEND_BYTES)(const void*, size_t);



unsigned char getByte()
{
    if( inputBytesLeft > 0 ) {
        unsigned char returnValue  = *input;
        input++;
        inputBytesLeft--;
        return returnValue;
    }

    return 0;
}

size_t getBytes( void* data, size_t numBytes )
{
    if( inputBytesLeft > 0 ) {
        *((char*)data)   = *input;
        input++;
        inputBytesLeft--;
        return 1;
    }

    return 0;
}

void putBytes(const void* data, size_t numBytes )
{
    char*   cData   = (char*)data;
    while( outputSpaceAvailable > 0 && numBytes > 0 ) {
        *output = *((char*)cData);
        output++;
        outputSpaceAvailable--;
        numBytes--;
        cData++;
    }
}





#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

/// error handler
void error(const char* msg)
{
  // smaller static binary than fprintf(stderr, "ERROR: %s\n", msg);
  fputs("ERROR: ", stderr);
  fputs(msg,       stderr);
  fputc('\n',      stderr);
  exit(1);
}


// ==================== I/O INTERFACE ====================


/// read a single byte (with simple buffering)
static unsigned char decompressorBytesIn()
{
}

/// write a block of bytes
static void decompressorBytesOut(const unsigned char* data, unsigned int numBytes)
{
}


// ==================== LZ4 DECOMPRESSOR ====================

/// decompress everything in input stream (accessed via getByte) and write to output stream (via sendBytes)
void unlz4(decompGET_BYTE getByte, decompSEND_BYTES sendBytes, const char* dictionary)
{
  // signature
  unsigned char signature1 = getByte();
  unsigned char signature2 = getByte();
  unsigned char signature3 = getByte();
  unsigned char signature4 = getByte();
  unsigned int  signature  = (signature4 << 24) | (signature3 << 16) | (signature2 << 8) | signature1;
  unsigned char isModern   = (signature == 0x184D2204);
  unsigned char isLegacy   = (signature == 0x184C2102);
  if (!isModern && !isLegacy)
    error("invalid signature");

  unsigned char hasBlockChecksum   = FALSE;
  unsigned char hasContentSize     = FALSE;
  unsigned char hasContentChecksum = FALSE;
  unsigned char hasDictionaryID    = FALSE;
  if (isModern)
  {
    // flags
    unsigned char flags = getByte();
    hasBlockChecksum   = flags & 16;
    hasContentSize     = flags &  8;
    hasContentChecksum = flags &  4;
    hasDictionaryID    = flags &  1;

    // only version 1 file format
    unsigned char version = flags >> 6;
    if (version != 1)
      error("only LZ4 file format version 1 supported");

    // ignore blocksize
    getByte();

    if (hasContentSize)
    {
      // ignore, skip 8 bytes
      getByte(); getByte(); getByte(); getByte();
      getByte(); getByte(); getByte(); getByte();
    }
    if (hasDictionaryID)
    {
      // ignore, skip 4 bytes
      getByte(); getByte(); getByte(); getByte();
    }

    // ignore header checksum (xxhash32 of everything up this point & 0xFF)
    getByte();
  }

  // don't lower this value, backreferences can be 64kb far away
#define HISTORY_SIZE 64*1024
  // contains the latest decoded data
  unsigned char history[HISTORY_SIZE];
  // next free position in history[]
  unsigned int  pos = 0;

  // dictionary compression is a recently introduced feature, just move its contents to the buffer
  if (dictionary != NULL)
  {
    // open dictionary
    FILE* dict = fopen(dictionary, "rb");
    if (!dict)
      error("cannot open dictionary");

    // get dictionary's filesize
    fseek(dict, 0, SEEK_END);
    long dictSize = ftell(dict);
    // only the last 64k are relevant
    long relevant = dictSize < 65536 ? 0 : dictSize - 65536;
    fseek(dict, relevant, SEEK_SET);
    if (dictSize > 65536)
      dictSize = 65536;
    // read it and store it at the end of the buffer
    fread(history + HISTORY_SIZE - dictSize, 1, dictSize, dict);
    fclose(dict);
  }

  // parse all blocks until blockSize == 0
  while (1)
  {
    // block size
    unsigned int blockSize = getByte();
    blockSize |= (unsigned int)getByte() <<  8;
    blockSize |= (unsigned int)getByte() << 16;
    blockSize |= (unsigned int)getByte() << 24;

    // highest bit set ?
    unsigned char isCompressed = isLegacy || (blockSize & 0x80000000) == 0;
    if (isModern)
      blockSize &= 0x7FFFFFFF;

    // stop after last block
    if (blockSize == 0)
      break;

    if (isCompressed)
    {
      // decompress block
      unsigned int blockOffset = 0;
      unsigned int numWritten  = 0;
      while (blockOffset < blockSize)
      {
        // get a token
        unsigned char token = getByte();
        blockOffset++;

        // determine number of literals
        unsigned int numLiterals = token >> 4;
        if (numLiterals == 15)
        {
          // number of literals length encoded in more than 1 byte
          unsigned char current;
          do
          {
            current = getByte();
            numLiterals += current;
            blockOffset++;
          } while (current == 255);
        }

        blockOffset += numLiterals;

        // copy all those literals
        if (pos + numLiterals < HISTORY_SIZE)
        {
          // fast loop
          while (numLiterals-- > 0)
            history[pos++] = getByte();
        }
        else
        {
          // slow loop
          while (numLiterals-- > 0)
          {
            history[pos++] = getByte();

            // flush output buffer
            if (pos == HISTORY_SIZE)
            {
              sendBytes(history, HISTORY_SIZE);
              numWritten += HISTORY_SIZE;
              pos = 0;
            }
          }
        }

        // last token has only literals
        if (blockOffset == blockSize)
          break;

        // match distance is encoded in two bytes (little endian)
        unsigned int delta = getByte();
        delta |= (unsigned int)getByte() << 8;
        // zero isn't allowed
        if (delta == 0)
          error("invalid offset");
        blockOffset += 2;

        // match length (always >= 4, therefore length is stored minus 4)
        unsigned int matchLength = 4 + (token & 0x0F);
        if (matchLength == 4 + 0x0F)
        {
          unsigned char current;
          do // match length encoded in more than 1 byte
          {
            current = getByte();
            matchLength += current;
            blockOffset++;
          } while (current == 255);
        }

        // copy match
        unsigned int referencePos = (pos >= delta) ? (pos - delta) : (HISTORY_SIZE + pos - delta);
        // start and end within the current 64k block ?
        if (pos + matchLength < HISTORY_SIZE && referencePos + matchLength < HISTORY_SIZE)
        {
          // read/write continuous block (no wrap-around at the end of history[])
          // fast copy
          if (pos >= referencePos + matchLength || referencePos >= pos + matchLength)
          {
            // non-overlapping
            memcpy(history + pos, history + referencePos, matchLength);
            pos += matchLength;
          }
          else
          {
            // overlapping, slower byte-wise copy
            while (matchLength-- > 0)
              history[pos++] = history[referencePos++];
          }
        }
        else
        {
          // either read or write wraps around at the end of history[]
          while (matchLength-- > 0)
          {
            // copy single byte
            history[pos++] = history[referencePos++];

            // cannot write anymore ? => wrap around
            if (pos == HISTORY_SIZE)
            {
              // flush output buffer
              sendBytes(history, HISTORY_SIZE);
              numWritten += HISTORY_SIZE;
              pos = 0;
            }
            // wrap-around of read location
            referencePos %= HISTORY_SIZE;
          }
        }
      }

      // all legacy blocks must be completely filled - except for the last one
      if (isLegacy && numWritten + pos < 8*1024*1024)
        break;
    }
    else
    {
      // copy uncompressed data and add to history, too (if next block is compressed and some matches refer to this block)
      while (blockSize-- > 0)
      {
        // copy a byte ...
        history[pos++] = getByte();
        // ... until buffer is full => send to output
        if (pos == HISTORY_SIZE)
        {
          sendBytes(history, HISTORY_SIZE);
          pos = 0;
        }
      }
    }

    if (hasBlockChecksum)
    {
      // ignore checksum, skip 4 bytes
      getByte(); getByte(); getByte(); getByte();
    }
  }

  if (hasContentChecksum)
  {
    // ignore checksum, skip 4 bytes
    getByte(); getByte(); getByte(); getByte();
  }

  // flush output buffer
  sendBytes(history, pos);
}








int main()
{
    // Compress
    input                   = &inputData[0];
    inputBytesLeft          = sizeof(inputData);
    output                  = &outputData[0];
    outputSpaceAvailable    = sizeof(outputData);

    smallz4::lz4( getBytes, putBytes );

    size_t  outputBytesUsed = sizeof(outputData) - outputSpaceAvailable;
    printf("outputBytesUsed=%zd\n",outputBytesUsed);
    for(int i=0; i<outputBytesUsed; i++) {
        const char    hex[] = "0123456789abcdef";
        char    t   = outputData[i];
        int     hi  = (t >> 4)&0xf;
        int     lo  = t &0xf;
        printf("0x%c%c, ", hex[hi], hex[lo]);
    }
    printf("\n");

    // Decompress
    input                   = &outputData[0];
    inputBytesLeft          = outputBytesUsed;
    output                  = &decompData[0];
    outputSpaceAvailable    = sizeof(decompData);

    unlz4( getByte, putBytes, NULL );

    outputBytesUsed = sizeof(decompData) - outputSpaceAvailable;
    printf("(decomp) outputBytesUsed=%zd\n",outputBytesUsed);
    for(int i=0; i<outputBytesUsed; i++) {
        const char    hex[] = "0123456789abcdef";
        char    t   = decompData[i];
        int     hi  = (t >> 4)&0xf;
        int     lo  = t &0xf;
        printf("0x%c%c, ", hex[hi], hex[lo]);
    }
    printf("\n");

    printf("[%s]\n", decompData);

}



