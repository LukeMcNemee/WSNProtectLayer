/*
 *      File with constants and structures. 
 *      Intended for early include into multiple components.
 *  	@author    Petr Svenda
 *  	@version   0.1
 *  	@date      2012-2013
 *  
 */
#ifndef PROTECTLAYERGLOBALS_H
#define PROTECTLAYERGLOBALS_H 

#include "message.h"

// Define to supress warning from printf function
#define DEBUG_PRINTF
#define NEW_PRINTF_SEMANTICS

#include "printf.h"
#include <stdarg.h>


typedef enum _error_values {
    EDIFFERENTKEY = 100,            /**< different key was used to protect message */
    EINVALIDDECRYPTION = 101,       /**< invalid format of message to be decrypted */
    ENOTALLKEYSDISCOVERED = 102,    /**< not all keys were discovered */
    EKEYNOTFOUND = 103              /**< requested key was not found */
} _error_values;

/**
        Various contants specified as enum to save stack space
*/
enum {
  AM_PROTECTLAYERRADIO = 128,
  AM_LOG_MSG = 129,
  AM_CON_GET_MSG = 130,
  AM_CON_SD_MSG = 131,
  AM_CON_PPCPD_MSG = 132,
  AM_CON_RPD_MSG = 133,
  AM_CON_KDCPD_MSG = 134,
  AM_FLASH_GET_MSG = 135,
  AM_FLASH_SET_MSG = 136,
  AM_INTRUSION_MSG = 137,
  POLICEMAN_TIMER_MESSAGE_MILLI = 5000,
  KEY_LENGTH = 16,
  MAX_NEIGHBOR_COUNT = 30,
  RECEIVE_BUFFER_LEN = 5,
  LOGGED_SIZE = 20,
  RSSI_THRESHOLD = -73
};
// NOTE: constants should be defined as item in enum above (to save space) #define MAX_NEIGHBOR_COUNT 	20 /**< Maximum number of neighbors - used to allocate static arrays */

#define FLAG_STATE_KDP_DISCOVERKEYS 0x0001	/**< neighbor keys discovery in progress */
#define FLAG_STATE_KDP_GETKEYTOBS   0x0002	/**< getting key to BS in progress */
#define FLAG_STATE_KDP_GETKEYTONODE 0x0004	/**< getting key to particular node in progress */

#define FLAG_STATE_CRYPTO_ENCRYPTION 0x0001	/**< encryption in progress (intentionally same as FLAG_STATE_CRYPTO_DECRYPTION) */
#define FLAG_STATE_CRYPTO_DECRYPTION 0x0001	/**< decryption in progress (intentionally same as FLAG_STATE_CRYPTO_ENCRYPTION) */
#define FLAG_STATE_CRYPTO_DERIV      0x0004	/**< key derivation in progress */
#define FLAG_STATE_CRYPTO_GENERATE   0x0008	/**< key generation in progress */

/**
	A structure representing request to send a message
*/
typedef struct _SendRequest {
/*@{*/
	uint16_t addr;	/**< target address */ 
	message_t* msg; /**< message to be send */
	uint8_t len;	/**< length of message to be send */
/*@}*/
} SendRequest_t;


/**
	A structure used to buffer incoming messages
*/
typedef struct _RecMsg {
/*@{*/
 	uint8_t isEmpty; 
	message_t* msg; /**< message to be received */
	void* payload;
	uint8_t len;	/**< length of message */
/*@}*/
} RecMsg_t;



/**
	A structure representing security header
*/
typedef nx_struct SPHeader {
/*@{*/
  nx_uint8_t msgType;	/**< type of message */
  nx_uint8_t privacyLevel;	/**< privacy level applied */
  nx_uint16_t sender;	/**< sender ID */
  nx_uint16_t receiver; /**< receiver ID */
/*@}*/
} SPHeader_t;

/**
	The enumeration of possible message type
*/
typedef enum _MSG_TYPE {
	MSG_OTHER = 0,	/**< message of other (unknown) type */
	MSG_APP = 1,	/**< application message */
	MSG_IDS = 2,	/**< message of intrusion detection component */
	MSG_KDC = 3,	/**< message of key distribution component */
	MSG_ROUTE = 4,	/**< message of routing component */
	MSG_PLEVEL = 5, /**< message specifiying privacy level */
	MSG_FORWARD = 6, /**< message to be forwarded to BS */ 
	MSG_IDSCOPY = 7, /**< copy of message to be passed to IDS from Privacy component */
	MSG_COUNT = 8   /**< number of message types */
} MSG_TYPE;


/**
	The enumeration of possible privacy levels
*/
typedef enum _PRIVACY_LEVEL {
	PLEVEL_0 = 0, /**< privacy level with no security */
	PLEVEL_1 = 1, /**< privacy level targeting attacker 1 */
	PLEVEL_2 = 2, /**< privacy level targeting attacker 2  */
	PLEVEL_3 = 3, /**< privacy level targeting attacker 3  */
	PLEVEL_4 = 4  /**< privacy level targeting attacker 4  */
} PRIVACY_LEVEL;
/**
	A structure representing a message that changes privacy level
*/
typedef nx_struct PLevelMsg {
	nx_uint8_t newPLevel; /**< new privacy level to be set */
} PLevelMsg_t;


typedef nx_struct AppMsg {
  nx_uint8_t appID;
  nx_uint8_t myType;
  nx_uint8_t info;
} AppMsg_t;

/**
	A structure representing message exchanged between IDSs
*/
typedef nx_struct IDSMsg {
/*@{*/
	// currently all broadcasted (reason for sender and receiver)
	// AM headers may be used instead, it's privacy question...
	nx_uint16_t sender;	/**< sender (TOS_NODE_ID) */
	nx_uint16_t receiver;	/**< receiver of this message */
	nx_uint16_t nodeID;	/**< ID of a node we send the reputation about */
	nx_uint8_t reputation;	/**< reputation */
/*@}*/	
} IDSMsg_t;

typedef nx_struct IntrusionMsg {
	nx_uint8_t counter;
} IntrusionMsg_t;

typedef enum _APP_MSG_TYPE {
	APPMSG_MOVEMENT = 1
} APP_MSG_TYPE;



typedef enum _APP_ID {
  APPID_POLICE = 1
} APP_ID;

/*
SavedData_t.txt
  targetID;SavedData.neigborID;KDCData.keyType;KDCData.keyValue;KDCData.dbgKeyID;IDSData.neighbor_reputation;IDSData.nb_messages;

PPCPrivData_t.txt
  targetID;PPCPrivData.priv_level;

RoutePrivData_t.txt
  targetID;RoutePrivData_t.parentNodeId;RoutePrivData_t.savedDataIdx;RoutePrivData_t.isValid;

KDCPrivData_t.txt
  targetID;KDCData.keyType;KDCData.keyValue;KDCData.dbgKeyID;

*/

enum {
	KEY_TOBS = 1,
	KEY_TONODE = 2,
} KEY_TYPE;

typedef nx_struct _key {
  nx_uint8_t    keyType;
  nx_uint8_t    keyValue[KEY_LENGTH];
  nx_uint16_t	dbgKeyID;
} PL_key_t;

typedef uint16_t node_id_t;

// Saved data that are backed up in EEPROM
// nx_struct only cause of the sending it via serial port
typedef nx_struct KDCData {
    PL_key_t shared_key;
} KDCData_t;

/**
	An IDS record about a neighbor
*/
typedef nx_struct IDSData {
/*@{*/
	nx_uint8_t neighbor_reputation;	/**< reputation of a neighbor */
	nx_uint8_t nb_messages;	/**< number of received messages */
/*@}*/
} IDSData_t;

/**
 * Data structure storing information about a neighbour saved by the protection layer component
 */
typedef nx_struct SavedData {
	nx_uint16_t nodeId; /**< id of the neighbouring node */
	KDCData_t kdcData; /**< information specific to the KDC component about the neighbour */
	IDSData_t idsData; /**< information specific to the IDS component about the neighbour */
} SavedData_t;

/**
 * Private data structure for the PPC component about this node
 */
typedef nx_struct PPCPrivData {
	nx_uint16_t priv_level; /**< current privacy level on this node */
} PPCPrivData_t;

/**
	Private data of the IDS
*/
typedef nx_struct IDSPrivData {
/*@{*/
	//PRIVACY_LEVEL priv_level;
/*@}*/
} IDSPrivData_t;

typedef nx_struct RoutePrivData {
	nx_uint16_t parentNodeId;
	nx_uint8_t savedDataIdx;
	nx_uint8_t isValid;
} RoutePrivData_t;

typedef nx_struct KDCPrivData {
    PL_key_t    keyToBS;
} KDCPrivData_t;
/**
 * Structure combining all the data that need to be stored on the node by the protection layer
 */
typedef struct CombinedData {
	SavedData_t savedData[MAX_NEIGHBOR_COUNT]; /**< an array of information about the node's neighbours */ 
	PPCPrivData_t ppcPrivData; /**< private data structure for the PPC component */
    RoutePrivData_t routePrivData;
    KDCPrivData_t kdcPrivData; /**< private data structure for the key distribution component */
} combinedData_t;

/**
 * Message structure for logging messages
 */

typedef nx_struct log_msg {
	nx_uint16_t counter; /**< counter of the log message */
	nx_uint8_t blockLength; /**< length of blocks required to be read from the flash memory at a time */
	nx_uint8_t data[LOGGED_SIZE];
} log_msg_t;

/**
 * Message structure for the configuration get request message
 */
typedef nx_struct con_get_msg {
	nx_uint16_t counter; /**< counter of the configuration get message */
} con_get_msg_t;

/**
 * Message structure for the configuration component setting or announcing saved data structure
 */
typedef nx_struct con_sd_msg {
	nx_uint16_t counter; /**< counter of the configuration setting message */
	nx_uint8_t savedDataIdx; /**< index of the savedData structure in the array */
	SavedData_t savedData; /**< the actual savedData structure with all the data */
} con_sd_msg_t;

/**
 * Message structure for the privacy protection layer component settings
 */
typedef nx_struct con_ppcpd_msg {
	nx_uint16_t counter; /**< counter of the privacy protection layer component settings message */ 
	PPCPrivData_t ppcPrivData; /**< the actual privacy protection layer data */
} con_ppcpd_msg_t;

typedef nx_struct con_rpd_msg {
	nx_uint16_t counter; /**< counter of the privacy protection layer component settings message */ 
	RoutePrivData_t rPrivData; /**< the actual privacy protection layer data */
} con_rpd_msg_t;

typedef nx_struct con_kdcpd_msg {
	nx_uint16_t counter; /**< counter of the privacy protection layer component settings message */ 
	KDCPrivData_t kdcPrivData; /**< the actual privacy protection layer data */
} con_kdcpd_msg_t;

/**
 * Message structure for the flash testing
 */
typedef nx_struct flash_get_msg {
	nx_uint16_t counter; /**< counter of the privacy protection layer component settings message */
} flash_get_msg_t;

/**
 * Message structure for the flash testing
 */
typedef nx_struct flash_set_msg {
	nx_uint16_t counter; /**< counter of the privacy protection layer component settings message */
} flash_set_msg_t;

typedef nx_struct intrusion_msg {
	nx_int8_t rssi;
} intrusion_msg_t;

/**
	Type for reputation of a neighbor
*/
typedef uint8_t NODE_REPUTATION;

/**
 * Enumeration of IDS status
 */
 typedef enum _IDS_STATUS {
	IDS_RESET = 0, //no security
	IDS_ON = 1, //
	IDS_OFF = 2  //
} IDS_STATUS;

void PrintDbg(const char* messageClass, const char* formatString, ...) {
    va_list args;
    va_start(args,formatString);
    dbg(formatString,args);
    va_end(args);

#ifdef DEBUG_PRINTF
    va_start(args,formatString);
    printf("%s: ", messageClass);
    printf(formatString,args);
    va_end(args);
    //printfflush();
#endif

}


#endif

