# 1 "d:\\Project\\Arduino\\STM32CORE\\snmp\\snmp.ino"
# 1 "d:\\Project\\Arduino\\STM32CORE\\snmp\\snmp.ino"
/**

* Agentuino SNMP Agent Library Prototyping...

*

* Copyright 2010 Eric C. Gionet <lavco_eg@hotmail.com>

*

*/
# 7 "d:\\Project\\Arduino\\STM32CORE\\snmp\\snmp.ino"
# 8 "d:\\Project\\Arduino\\STM32CORE\\snmp\\snmp.ino" 2
# 9 "d:\\Project\\Arduino\\STM32CORE\\snmp\\snmp.ino" 2
# 10 "d:\\Project\\Arduino\\STM32CORE\\snmp\\snmp.ino" 2
//#include <MemoryFree.h>
# 12 "d:\\Project\\Arduino\\STM32CORE\\snmp\\snmp.ino" 2
# 13 "d:\\Project\\Arduino\\STM32CORE\\snmp\\snmp.ino" 2
//

//
static byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static byte ip[] = { 192, 168, 1, 8 };
static byte gateway[] = { 192, 168, 1, 1 };
static byte subnet[] = { 255, 255, 255, 0 };
//
// tkmib - linux mib browser
//
// RFC1213-MIB OIDs
// .iso (.1)
// .iso.org (.1.3)
// .iso.org.dod (.1.3.6)
// .iso.org.dod.internet (.1.3.6.1)
// .iso.org.dod.internet.mgmt (.1.3.6.1.2)
// .iso.org.dod.internet.mgmt.mib-2 (.1.3.6.1.2.1)
// .iso.org.dod.internet.mgmt.mib-2.system (.1.3.6.1.2.1.1)
// .iso.org.dod.internet.mgmt.mib-2.system.sysDescr (.1.3.6.1.2.1.1.1)
static char sysDescr[] = "1.3.6.1.2.1.1.1.0"; // read-only  (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysObjectID (.1.3.6.1.2.1.1.2)
static char sysObjectID[] = "1.3.6.1.2.1.1.2.0"; // read-only  (ObjectIdentifier)
// .iso.org.dod.internet.mgmt.mib-2.system.sysUpTime (.1.3.6.1.2.1.1.3)
static char sysUpTime[] = "1.3.6.1.2.1.1.3.0"; // read-only  (TimeTicks)
// .iso.org.dod.internet.mgmt.mib-2.system.sysContact (.1.3.6.1.2.1.1.4)
static char sysContact[] = "1.3.6.1.2.1.1.4.0"; // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysName (.1.3.6.1.2.1.1.5)
static char sysName[] = "1.3.6.1.2.1.1.5.0"; // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysLocation (.1.3.6.1.2.1.1.6)
static char sysLocation[] = "1.3.6.1.2.1.1.6.0"; // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysServices (.1.3.6.1.2.1.1.7)
static char sysServices[] = "1.3.6.1.2.1.1.7.0"; // read-only  (Integer)
//
// Arduino defined OIDs
// .iso.org.dod.internet.private (.1.3.6.1.4)
// .iso.org.dod.internet.private.enterprises (.1.3.6.1.4.1)
// .iso.org.dod.internet.private.enterprises.arduino (.1.3.6.1.4.1.36582)
//
//
// RFC1213 local values
static char locDescr[] = "Agentuino, a light-weight SNMP Agent."; // read-only (static)
static char locObjectID[] = "1.3.6.1.3.2009.0"; // read-only (static)
static uint32_t locUpTime = 0; // read-only (static)
static char locContact[20] = "Eric Gionet"; // should be stored/read from EEPROM - read/write (not done for simplicity)
static char locName[20] = "Agentuino"; // should be stored/read from EEPROM - read/write (not done for simplicity)
static char locLocation[20] = "Nova Scotia, CA"; // should be stored/read from EEPROM - read/write (not done for simplicity)
static int32_t locServices = 7; // read-only (static)

uint32_t prevMillis = millis();
char oid[64 /* 128*/];
SNMP_API_STAT_CODES api_status;
SNMP_ERR_CODES status;

void pduReceived()
{
  SNMP_PDU pdu;
  //
//  #ifdef DEBUG
//    Serial1 << F("UDP Packet Received Start..") << F(" RAM:") << freeMemory() << endl;
//  #endif
  //

  api_status = Agentuino.requestPdu(&pdu);
  Serial1.println(pdu.type);
  //
  if ( pdu.type == SNMP_PDU_GET || pdu.type == SNMP_PDU_GET_NEXT || pdu.type == SNMP_PDU_SET
    && pdu.error == SNMP_ERR_NO_ERROR && api_status == SNMP_API_STAT_SUCCESS ) {
    //
    pdu.OID.toString(oid);

    Serial1.println(oid);
    //
    //Serial1 << "OID: " << oid << endl;
    //
    if ( strcmp((oid), (sysDescr)) == 0 ) {
      // handle sysDescr (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read-only
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        // response packet from get-request - locDescr
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locDescr);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //

        Serial1 << (_FLASH_STRING(("sysDescr...")).Printable()) << locDescr << (_FLASH_STRING((" ")).Printable()) << pdu.VALUE.size << endl;

    } else if ( strcmp((oid), (sysUpTime)) == 0 ) {
      // handle sysName (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read-only
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        // response packet from get-request - locUpTime
        status = pdu.VALUE.encode(SNMP_SYNTAX_TIME_TICKS, locUpTime);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
        Serial1.println(status);
      }
      //

        Serial1 << (_FLASH_STRING(("sysUpTime...")).Printable()) << locUpTime << (_FLASH_STRING((" ")).Printable()) << pdu.VALUE.size << endl;

    } else if ( strcmp((oid), (sysName)) == 0 ) {
      // handle sysName (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read/write
        status = pdu.VALUE.decode(locName, strlen(locName));
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        // response packet from get-request - locName
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locName);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //

        Serial1 << (_FLASH_STRING(("sysName...")).Printable()) << locName << (_FLASH_STRING((" ")).Printable()) << pdu.VALUE.size << endl;

    } else if ( strcmp((oid), (sysContact)) == 0 ) {
      Serial1.println("sysContact");
      // handle sysContact (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read/write
        status = pdu.VALUE.decode(locContact, strlen(locContact));
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        // response packet from get-request - locContact
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locContact);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //

        Serial1 << (_FLASH_STRING(("sysContact...")).Printable()) << locContact << (_FLASH_STRING((" ")).Printable()) << pdu.VALUE.size << endl;

    } else if ( strcmp((oid), (sysLocation)) == 0 ) {
      // handle sysLocation (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read/write
        status = pdu.VALUE.decode(locLocation, strlen(locLocation));
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        // response packet from get-request - locLocation
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locLocation);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //

        Serial1 << (_FLASH_STRING(("sysLocation...")).Printable()) << locLocation << (_FLASH_STRING((" ")).Printable()) << pdu.VALUE.size << endl;

    } else if ( strcmp((oid), (sysServices)) == 0 ) {
      // handle sysServices (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read-only
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        // response packet from get-request - locServices
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, locServices);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //

        Serial1 << (_FLASH_STRING(("  locServices...")).Printable()) << locServices << (_FLASH_STRING((" ")).Printable()) << pdu.VALUE.size << endl;

    } else {
      // oid does not exist
      //
      // response packet - object not found
      pdu.type = SNMP_PDU_RESPONSE;
      pdu.error = SNMP_ERR_NO_SUCH_NAME;
    }
    //
    Agentuino.responsePdu(&pdu);
  }
  //
  Agentuino.freePdu(&pdu);
  //
  //Serial1 << "UDP Packet Received End.." << " RAM:" << freeMemory() << endl;
}

void setup()
{
  Serial1.begin(115200);
  Ethernet.begin(mac, ip);
  //
  api_status = Agentuino.begin();
  Serial1.println(api_status);
  //
  if ( api_status == SNMP_API_STAT_SUCCESS ) {
    //
    Agentuino.onPduReceive(pduReceived);
    //
    delay(10);
    //
    Serial1 << (_FLASH_STRING(("SNMP Agent Initalized...")).Printable()) << endl;
    //
    return;
  }
  //
  delay(10);
  //
  Serial1 << (_FLASH_STRING(("SNMP Agent Initalization Problem...")).Printable()) << status << endl;
}

void loop()
{
  // listen/handle for incoming SNMP requests
  Agentuino.listen();
  //
  // sysUpTime - The time (in hundredths of a second) since
  // the network management portion of the system was last
  // re-initialized.
  if ( millis() - prevMillis > 1000 ) {
    // increment previous milliseconds
    prevMillis += 1000;
    //
    // increment up-time counter
    locUpTime += 100;
  }
}
