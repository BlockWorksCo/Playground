//
// --------------------------------------------------------------------------
//  Gurux Ltd
// 
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux Device Framework.
//
// Gurux Device Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2. 
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------
package gurux.dlms.client;

import java.util.ArrayList;
import java.util.Map;


import gurux.dlms.TranslatorOutputType;
import gurux.dlms.GXDLMSTranslator;
import gurux.common.GXCmdParameter;
import gurux.common.GXCommon;
import gurux.common.enums.TraceLevel;
import gurux.dlms.GXSimpleEntry;
import gurux.dlms.enums.Authentication;
import gurux.dlms.enums.InterfaceType;
import gurux.dlms.enums.ObjectType;
import gurux.io.BaudRate;
import gurux.io.Parity;
import gurux.io.StopBits;
import gurux.net.GXNet;
import gurux.serial.GXSerial;

public class sampleclient {
    /**
     * @param args
     *            the command line arguments
     */
    public static void main(String[] args) {
        Settings settings = new Settings();

        String              pdu;
        String              xml;
        String              message;
        GXDLMSTranslator    translator = new GXDLMSTranslator(TranslatorOutputType.SIMPLE_XML);

        pdu = args[0];
        System.out.println(pdu);

        xml = translator.pduToXml(pdu);
        System.out.println(xml);
        
/*
        //
        //
        //
        xml = translator.pduToXml("60 1D A1 09 06 07 60 85 74 05 08 01 01 BE 10 04 0E 01 00 00 00 06 5F 1F 04 00 18 02 20 1F FF");
        System.out.println(xml);

        //
        //
        //
        message = "C1 01 09 00 08 00 00 01 00 00 FF 02 00 09 0C 07 D2 0C 04 03 0A 06 0B FF 00 78 00";
        //xml = translator.messageToXml( message.getBytes() );
        xml = translator.pduToXml( message );
        System.out.println(xml);

        //
        //
        //
        //message = "7E A0 19 95 75 54 68 35 E6 E6 00 C0 01 81 00 08 00 00 01 00 00 FF 01 00 0D FD 7E";
        message = "C0 01 81 00 08 00 00 01 00 00 FF 01 00";
        xml = translator.pduToXml( message );
        System.out.println(xml);

        //
        //
        //
        xml="<HDLC len=\"57\" > <TargetAddress Value=\"16\" /> <SourceAddress Value=\"16401\" /> <PDU> <AssociationResponse> <ApplicationContextName Value=\"LN\" /> <AssociationResult Value=\"0\" /> <ResultSourceDiagnostic> <ACSEServiceUser Value=\"0\" /> </ResultSourceDiagnostic> <InitiateResponse> <NegotiatedDlmsVersionNumber Value=\"6\" /> <NegotiatedConformance> <ConformanceBit Name=\"Action\" /> <ConformanceBit Name=\"SelectiveAccess\" /> <ConformanceBit Name=\"Set\" /> <ConformanceBit Name=\"Get\" /> <ConformanceBit Name=\"BlockTransferWithGetOrRead\" /> <ConformanceBit Name=\"Attribute0SupportedWithGet\" /> </NegotiatedConformance> <NegotiatedMaxPduSize Value=\"6400\" /> <VaaName Value=\"7\" /> </InitiateResponse> </AssociationResponse> </PDU> </HDLC>";
        pdu = translator.xmlToHexPdu(xml);
        System.out.println(pdu);

        message = "c4028100000000080081a012000509060100030601ff0f02120000020412000509060100030602ff0f02120000020412000509060100030603ff0f02120000020412000509060100030604ff0f02120000020412000509060100040600ff0f02120000020412000509060100040601ff0f02120000020412000509060100040602ff0f02120000020412000509060100040603ff0f02120000020412000509060100040604ff0f02120000";
        xml = translator.pduToXml( message );
        System.out.println(xml);
*/
    }

    /**
     * Show help.
     */
    static void showHelp() {
        System.out.println(
                "GuruxDlmsSample reads data from the DLMS/COSEM device.");
        System.out.println(
                "GuruxDlmsSample -h [Meter IP Address] -p [Meter Port No] -c 16 -s 1 -r SN");
        System.out.println(" -h \t host name or IP address.");
        System.out.println(" -p \t port number or name (Example: 1000).");
        System.out.println(" -S \t serial port.");
        System.out.println(" -i IEC is a start protocol.");
        System.out.println(" -a \t Authentication (None, Low, High).");
        System.out.println(" -P \t Password for authentication.");
        System.out.println(" -c \t Client address. (Default: 16)");
        System.out.println(" -s \t Server address. (Default: 1)");
        System.out.println(" -n \t Server address as serial number.");
        System.out.println(
                " -r [sn, sn]\t Short name or Logican Name (default) referencing is used.");
        System.out.println(" -w WRAPPER profile is used. HDLC is default.");
        System.out
                .println(" -t [Error, Warning, Info, Verbose] Trace messages.");
        System.out.println(
                " -g \"0.0.1.0.0.255:1; 0.0.1.0.0.255:2\" Get selected object(s) with given attribute index.");
        System.out.println("Example:");
        System.out.println("Read LG device using TCP/IP connection.");
        System.out.println(
                "GuruxDlmsSample -r SN -c 16 -s 1 -h [Meter IP Address] -p [Meter Port No]");
        System.out.println("Read LG device using serial port connection.");
        System.out.println("GuruxDlmsSample -r SN -c 16 -s 1 -sp COM1 -i");
        System.out.println("Read Indian device using serial port connection.");
        System.out.println(
                "GuruxDlmsSample -S COM1 -c 16 -s 1 -a Low -P [password]");

    }

    static int getParameters(String[] args, Settings settings) {
        ArrayList<GXCmdParameter> parameters =
                GXCommon.getParameters(args, "h:p:c:s:r:it:a:p:wP:g:");
        GXNet net = null;
        for (GXCmdParameter it : parameters) {
            switch (it.getTag()) {
            case 'w':
                settings.client.setInterfaceType(InterfaceType.WRAPPER);
                break;
            case 'r':
                if ("sn".compareTo(it.getValue()) == 0) {
                    settings.client.setUseLogicalNameReferencing(false);
                } else if ("ln".compareTo(it.getValue()) == 0) {
                    settings.client.setUseLogicalNameReferencing(true);
                } else {
                    throw new IllegalArgumentException(
                            "Invalid reference option.");
                }
                break;
            case 'h':
                // Host address.
                if (settings.media == null) {
                    settings.media = new GXNet();
                }
                net = (GXNet) settings.media;
                net.setHostName(it.getValue());
                break;
            case 't':
                // Trace.
                if ("Error".compareTo(it.getValue()) == 0)
                    settings.trace = TraceLevel.ERROR;
                else if ("Warning".compareTo(it.getValue()) == 0)
                    settings.trace = TraceLevel.WARNING;
                else if ("Info".compareTo(it.getValue()) == 0)
                    settings.trace = TraceLevel.INFO;
                else if ("Verbose".compareTo(it.getValue()) == 0)
                    settings.trace = TraceLevel.VERBOSE;
                else if ("Off".compareTo(it.getValue()) == 0)
                    settings.trace = TraceLevel.OFF;
                else
                    throw new IllegalArgumentException(
                            "Invalid Authentication option. (Error, Warning, Info, Verbose, Off).");
                break;
            case 'p':
                // Port.
                if (settings.media == null) {
                    settings.media = new GXNet();
                }
                net = (GXNet) settings.media;
                net.setPort(Integer.parseInt(it.getValue()));
                break;
            case 'P':// Password
                settings.client.setPassword(it.getValue().getBytes());
                break;
            case 'i':
                // IEC.
                settings.iec = true;
                break;
            case 'g':
                // Get (read) selected objects.
                for (String o : it.getValue().split("[;,]")) {
                    String[] tmp = o.split("[:]");
                    if (tmp.length != 2) {
                        throw new IllegalArgumentException(
                                "Invalid Logical name or attribute index.");
                    }
                    settings.readObjects.add(new GXSimpleEntry<String, Integer>(
                            tmp[0].trim(), Integer.parseInt(tmp[1].trim())));
                }
                break;
            case 'S':// Serial Port
                settings.media = new GXSerial();
                GXSerial serial = (GXSerial) settings.media;
                serial.setPortName(it.getValue());
                break;
            case 'a':
                try {
                    settings.client.setAuthentication(
                            Authentication.valueOfString(it.getValue()));
                } catch (Exception e) {
                    throw new IllegalArgumentException(
                            "Invalid Authentication option: '" + it.getValue()
                                    + "'. (None, Low, High, HighMd5, HighSha1, HighGmac, HighSha256)");
                }
                break;
            case 'o':
                break;
            case 'c':
                settings.client
                        .setClientAddress(Integer.parseInt(it.getValue()));
                break;
            case 's':
                settings.client
                        .setServerAddress(Integer.parseInt(it.getValue()));
                break;
            case '?':
                switch (it.getTag()) {
                case 'c':
                    throw new IllegalArgumentException(
                            "Missing mandatory client option.");
                case 's':
                    throw new IllegalArgumentException(
                            "Missing mandatory server option.");
                case 'h':
                    throw new IllegalArgumentException(
                            "Missing mandatory host name option.");
                case 'p':
                    throw new IllegalArgumentException(
                            "Missing mandatory port option.");
                case 'r':
                    throw new IllegalArgumentException(
                            "Missing mandatory reference option.");
                case 'a':
                    throw new IllegalArgumentException(
                            "Missing mandatory authentication option.");
                case 'S':
                    throw new IllegalArgumentException(
                            "Missing mandatory Serial port option.\n");
                case 't':
                    throw new IllegalArgumentException(
                            "Missing mandatory trace option.\n");
                default:
                    showHelp();
                    return 1;
                }
            default:
                showHelp();
                return 1;
            }
        }
        if (settings.media == null) {
            showHelp();
            return 1;
        }
        return 0;
    }
}
