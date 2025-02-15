/**
 ****************************************************************************************
 * @addtogroup Core_Modules
 * @{
 * @addtogroup NVDS
 * @brief Non Volatile Data Storage (NVDS)
 *
 *   Parameters management
 *   there are two compilation options:
 *     + NVDS_8BIT_TAGLENGTH :
 *       if set, each TAG has a maximum length of 256 bytes
 *       if not set, each TAG has a maximum length of 65536 bytes
 *     + NVDS_PACKED :
 *       if not set, all the TAG header structures and TAG data contents are stored with an
 *       alignment on 32 bit boundary
 *       if set, all the TAG header structures and TAG data contents are stored
 *       consecutively without gaps (as would be a structure with pragma packed)
 *     + NVDS_READ_WRITE :
 *       if not set, only GET action on TAGs is provided.
 *       if set, PUT/DEL/LOCK actions are provided in addition of GET action.
 *
 * @{
 *
 * @file nvds.h
 *
 * @brief Non Volatile Data Storage (NVDS) driver
 *
 * Copyright (C) RivieraWaves 2009-2014
 * Copyright (C) 2017- 2019 Modified by Dialog Semiconductor
 *
 ****************************************************************************************
 */
#ifndef _NVDS_H_
#define _NVDS_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h>           // boolean definition
#include <stdint.h>            // integer definition


/*
 * DEFINES
 ****************************************************************************************
 */

/// NVDS is defined as read-write
#define NVDS_READ_WRITE          1

/// NVDS is defined as packed
#define NVDS_PACKED              1

/// NVDS has 8-bit length tags
#define NVDS_8BIT_TAGLENGTH      1

/// Type of the tag length (8 or 16 bits)
#if (NVDS_8BIT_TAGLENGTH)
typedef uint8_t nvds_tag_len_t;
#else
typedef uint16_t nvds_tag_len_t;
#endif // NVDS_8BIT_TAGLENGTH


/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */

/// Possible Returned Status
enum NVDS_STATUS
{
    /// NVDS status OK
    NVDS_OK,
    /// generic NVDS status KO
    NVDS_FAIL,
    /// NVDS TAG unrecognized
    NVDS_TAG_NOT_DEFINED,
    /// No space for NVDS
    NVDS_NO_SPACE_AVAILABLE,
    /// Length violation
    NVDS_LENGTH_OUT_OF_RANGE,
    /// NVDS parameter locked
    NVDS_PARAM_LOCKED,
    /// NVDS corrupted
    NVDS_CORRUPT
};

/// List of NVDS TAG identifiers
enum NVDS_TAG
{
    /// Definition of the tag associated to each parameters
    /// Local Bd Address
    NVDS_TAG_BD_ADDRESS                 = 0x01,
    /// Device Name
    NVDS_TAG_DEVICE_NAME                = 0x02,
    /// Radio Drift
    NVDS_TAG_LPCLK_DRIFT                = 0x07,
    /// Radio Jitter
    NVDS_TAG_LPCLK_JITTER               = 0x08,
    /// Radio Class
    NVDS_TAG_RADIO_CLASS                = 0x09,
    /// Bluejay specific Settings
    NVDS_TAG_BJ_TXCNTL1                 = 0x0A,
    /// BLE Application Advertising data
    NVDS_TAG_APP_BLE_ADV_DATA           = 0x0B,
    /// BLE Application Scan response data
    NVDS_TAG_APP_BLE_SCAN_RESP_DATA     = 0x0C,
    /// External wake-up time
    NVDS_TAG_EXT_WAKEUP_TIME            = 0x0D,
    /// Oscillator wake-up time
    NVDS_TAG_OSC_WAKEUP_TIME            = 0x0E,
    /// Radio wake-up time
    NVDS_TAG_RM_WAKEUP_TIME             = 0x0F,
    /// UART baudrate
    NVDS_TAG_UART_BAUDRATE              = 0x10,
    /// Enable sleep mode
    NVDS_TAG_SLEEP_ENABLE               = 0x11,
    /// Enable External Wakeup
    NVDS_TAG_EXT_WAKEUP_ENABLE          = 0x12,
    /// SP Private Key
    NVDS_TAG_SP_PRIVATE_KEY             = 0x13,
    /// SP Public Key
    NVDS_TAG_SP_PUBLIC_KEY              = 0x14,
    /// Errata adopted check
    NVDS_TAG_ERRATA_ADOPTED             = 0x15,
    /// CQDDR Tags
    NVDS_TAG_BASIC_THRESHOLD            = 0x16,
    NVDS_TAG_EDR_THRESHOLD              = 0x17,
    NVDS_TAG_BASIC_ALGORITHM            = 0x18,
    NVDS_TAG_EDR_ALGORITHM              = 0x19,
    NVDS_TAG_BASIC_PACKET_LUT           = 0x2A,
    NVDS_TAG_EDR_PACKET_LUT             = 0x2B,
    /// Synchronous links configuration
    NVDS_TAG_SYNC_CONFIG                = 0x2C,
    /// PCM Settings
    NVDS_TAG_PCM_SETTINGS               = 0x2D,
    /// Default size used to transmit synchronous data over HCI
    NVDS_TAG_HCI_SYNC_BUF_SIZE          = 0x2E,

    /// Diagport configuration
    NVDS_TAG_DIAG_BT_HW                 = 0x30,
    NVDS_TAG_DIAG_BLE_HW                = 0x31,
    NVDS_TAG_DIAG_SW                    = 0x32,
    NVDS_TAG_DIAG_PLF                   = 0x34,

    /// Mouse Sample Rate
    NVDS_TAG_MOUSE_SAMPLE_RATE          = 0x38,
    /// Peripheral Bonded
    NVDS_TAG_PERIPH_BONDED              = 0x39,
    /// Mouse NTF Cfg
    NVDS_TAG_MOUSE_NTF_CFG              = 0x3A,
    /// Mouse Timeout value
    NVDS_TAG_MOUSE_TIMEOUT              = 0x3B,
    /// Peer Device BD Address
    NVDS_TAG_PEER_BD_ADDRESS            = 0x3C,
    /// Mouse Energy Safe
    NVDS_TAG_MOUSE_ENERGY_SAFE          = 0x3D,
    /// EDIV (2bytes), RAND NB (8bytes),  LTK (16 bytes), Key Size (1 byte)
    NVDS_TAG_LTK                        = 0x3E,
    /// PAIRING
    NVDS_TAG_PAIRING                    = 0x3F,

    /// BLE Channel Assessment tags
    NVDS_TAG_BLE_CA_TIMER_DUR           = 0x40,
    NVDS_TAG_BLE_CRA_TIMER_DUR          = 0x41,
    NVDS_TAG_BLE_CA_MIN_RSSI            = 0x42,
    NVDS_TAG_BLE_CA_NB_PKT              = 0x43,
    NVDS_TAG_BLE_CA_NB_BAD_PKT          = 0x44,

    /// AFH algorithm tags
    NVDS_TAG_AFH_REASS_NBCH             = 0x51,
    NVDS_TAG_AFH_WINLGTH                = 0x52,
    NVDS_TAG_AFH_RSSIMIN                = 0x53,
    NVDS_TAG_AFH_PERTHRESBAD            = 0x54,
    NVDS_TAG_AFH_REASS_INT              = 0x55,
    NVDS_TAG_AFH_NMIN                   = 0x56,
    NVDS_TAG_AFH_MAXADAPT               = 0x57,
    NVDS_TAG_AFH_THSMIN                 = 0x58,


    NVDS_TAG_BT_LINK_KEY_FIRST          = 0x60,
    NVDS_TAG_BT_LINK_KEY_LAST           = 0x67,

    NVDS_TAG_BLE_LINK_KEY_FIRST         = 0x70,
    NVDS_TAG_BLE_LINK_KEY_LAST          = 0x7F,
};

/// List of NVDS Tag lengths
enum NVDS_LEN
{
    // Definition of length associated to each parameters
    /// Local Bd Address
    NVDS_LEN_BD_ADDRESS                 = 6,
    /// Device Name
    NVDS_LEN_DEVICE_NAME                = 248,
    /// Low power clock drift
    NVDS_LEN_LPCLK_DRIFT                = 2,
    /// Low power clock jitter
    NVDS_LEN_LPCLK_JITTER               = 1,
    /// Radio Class
    NVDS_LEN_RADIO_CLASS                = 1,
    /// Bluejay specific Settings
    NVDS_LEN_BJ_TXCNTL1                 = 4,
    /// BLE Application Advertising data
    NVDS_LEN_APP_BLE_ADV_DATA           = 32,
    /// BLE Application Scan response data
    NVDS_LEN_APP_BLE_SCAN_RESP_DATA     = 32,
    /// External wake-up time
    NVDS_LEN_EXT_WAKEUP_TIME            = 2,
    /// Oscillator wake-up time
    NVDS_LEN_OSC_WAKEUP_TIME            = 2,
    /// Radio wake-up time
    NVDS_LEN_RM_WAKEUP_TIME             = 2,
    /// UART baudrate
    NVDS_LEN_UART_BAUDRATE              = 4,
    /// Enable sleep mode
    NVDS_LEN_SLEEP_ENABLE               = 1,
    /// Enable External Wakeup
    NVDS_LEN_EXT_WAKEUP_ENABLE          = 1,
    /// SP Private Key
    NVDS_LEN_SP_PRIVATE_KEY             = 24,
    /// SP Public Key
    NVDS_LEN_SP_PUBLIC_KEY              = 48,
    /// Errata adopted check
    NVDS_LEN_ERRATA_ADOPTED             = 1,
    /// CQDDR Tags
    NVDS_LEN_BASIC_THRESHOLD            = 70,
    NVDS_LEN_EDR_THRESHOLD              = 70,
    NVDS_LEN_BASIC_ALGORITHM            = 21,
    NVDS_LEN_EDR_ALGORITHM              = 21,
    NVDS_LEN_BASIC_PACKET_LUT           = 16,
    NVDS_LEN_EDR_PACKET_LUT             = 16,
    /// Synchronous links configuration
    NVDS_LEN_SYNC_CONFIG                = 2,
    /// PCM Settings
    NVDS_LEN_PCM_SETTINGS               = 4,
    /// Default size used to transmit synchronous data over HCI
    NVDS_LEN_HCI_SYNC_BUF_SIZE          = 1,
    /// Diagport configuration
    NVDS_LEN_DIAG_BT_HW                 = 4,
    NVDS_LEN_DIAG_BLE_HW                = 4,
    NVDS_LEN_DIAG_SW                    = 4,
    NVDS_LEN_DIAG_PLF                   = 4,

    /// Mouse sample rate
    NVDS_LEN_MOUSE_SAMPLE_RATE          = 1,
    /// Peripheral Bonded
    NVDS_LEN_PERIPH_BONDED              = 1,
    /// Mouse MTF Cfg
    NVDS_LEN_MOUSE_NTF_CFG              = 2,
    /// Mouse Timeout
    NVDS_LEN_MOUSE_TIMEOUT              = 2,
    /// Peer device BD Address + Address Type
    NVDS_LEN_PEER_BD_ADDRESS            = 7,
    /// Mouse Safe Energy
    NVDS_LEN_MOUSE_SAFE_ENERGY          = 2,
    /// LTK
    NVDS_LEN_LTK                        = 28,
    /// PAIRING
    NVDS_LEN_PAIRING                    = 54,

    NVDS_LEN_BLE_CA_TIMER_DUR           = 2,
    NVDS_LEN_BLE_CRA_TIMER_DUR          = 1,
    NVDS_LEN_BLE_CA_MIN_RSSI            = 1,
    NVDS_LEN_BLE_CA_NB_PKT              = 1,
    NVDS_LEN_BLE_CA_NB_BAD_PKT          = 1,

    /// AFH algorithm tags
    NVDS_LEN_AFH_REASS_NBCH             = 1,
    NVDS_LEN_AFH_WINLGTH                = 1,
    NVDS_LEN_AFH_RSSIMIN                = 1,
    NVDS_LEN_AFH_PERTHRESBAD            = 1,
    NVDS_LEN_AFH_REASS_INT              = 1,
    NVDS_LEN_AFH_NMIN                   = 1,
    NVDS_LEN_AFH_MAXADAPT               = 1,
    NVDS_LEN_AFH_THSMIN                 = 1,
    /// Link keys
    NVDS_LEN_BT_LINK_KEY                = 22,
    NVDS_LEN_BLE_LINK_KEY               = 48,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

#if defined (__DA14531__) && !defined (__EXCLUDE_ROM_NVDS__)

/// NVDS configuration
typedef struct
{
    /// Data storage
    const struct nvds_data_struct *nvds_data_storage;
    /// Device address
    struct bd_addr *dev_bdaddr;
} rom_nvds_cfg_t;
#endif

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Look for a specific tag and return, if found and matching (in length), the
 *        DATA part of the TAG.
 *
 * If the length does not match, the TAG header structure is still filled, in order for
 * the caller to be able to check the actual length of the TAG.
 *
 * @param[in]  tag          TAG to look for whose DATA is to be retrieved
 * @param[in]  lengthPtr    Expected length of the TAG
 * @param[out] buf          A pointer to the buffer allocated by the caller to be filled with
 *                          the DATA part of the TAG
 *
 * @return  NVDS_OK                  The read operation was performed
 *          NVDS_LENGTH_OUT_OF_RANGE The length passed in parameter is different than the TAG's
 ****************************************************************************************
 */
uint8_t nvds_get_func(uint8_t tag, nvds_tag_len_t *lengthPtr, uint8_t *buf);

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize NVDS.
 * @param[in]  base         Not used
 * @param[in]  len          Not used
 * @return NVDS_OK
 ****************************************************************************************
 */
uint8_t nvds_init(uint8_t *base, uint32_t len);

/**
 ****************************************************************************************
 * @brief Look for a specific tag and return, if found and matching (in length), the
 *        DATA part of the TAG.
 *
 * If the length does not match, the TAG header structure is still filled, in order for
 * the caller to be able to check the actual length of the TAG.
 *
 * @param[in]  tag          TAG to look for whose DATA is to be retrieved
 * @param[in]  lengthPtr    Expected length of the TAG
 * @param[out] buf          A pointer to the buffer allocated by the caller to be filled with
 *                          the DATA part of the TAG
 *
 * @return  NVDS_OK                  The read operation was performed
 *          NVDS_LENGTH_OUT_OF_RANGE The length passed in parameter is different than the TAG's
 ****************************************************************************************
 */
uint8_t nvds_get(uint8_t tag, nvds_tag_len_t * lengthPtr, uint8_t *buf);

#if (NVDS_READ_WRITE == 1)

/**
 ****************************************************************************************
 * @brief Look for a specific tag and delete it (Status set to invalid)
 *
 * Implementation notes
 * 1. The write function call return status is not handled
 *
 * @param[in]  tag    TAG to mark as deleted
 *
 * @return NVDS_FAIL
 ****************************************************************************************
 */
uint8_t nvds_del(uint8_t tag);

/**
 ****************************************************************************************
 * @brief Look for a specific tag and lock it (Status lock bit set to LOCK).
 *
 * The write function call return status is not handled
 *
 * @param[in]  tag    TAG to mark as locked
 *
 * @return NVDS_OK    TAG found and locked
 *         (others)        return values from function call nvds_browse_tag
 ****************************************************************************************
 */
uint8_t nvds_lock(uint8_t tag);

/**
 ****************************************************************************************
 * @brief This function adds a specific TAG to the NVDS.
 *
 * Steps:
 * 1) parse all the TAGs to:
 * 1.1) calculate the total size of all the valid TAGs
 * 1.2) erase the existing TAGs that have the same ID
 * 1.3) check if we can use the same TAG area in case of an EEPROM
 * 1.4) check that the TAG is not locked
 * 2) if we have to add the new TAG at the end for the NVDS (cant use same area):
 * 2.1) allocate the appropriate amount of memory
 * 2.2) purge the NVDS
 * 2.3) free the memory allocated
 * 2.4) check that there is now enough room for the new TAG or return
 *      NO_SPACE_AVAILABLE
 * 3) add the new TAG
 *
 * @param[in]  tag     TAG to look for whose DATA is to be retrieved
 * @param[in]  length  Expected length of the TAG
 * @param[in]  buf     Pointer to the buffer containing the DATA part of the TAG to add to
 *                     the NVDS
 *
 * @return NVDS_OK                  New TAG correctly written to the NVDS
 *         NVDS_PARAM_LOCKED        New TAG is trying to overwrite a TAG that is locked
 *         NO_SPACE_AVAILABLE       New TAG can not fit in the available space in the NVDS
 ****************************************************************************************
 */
uint8_t nvds_put(uint8_t tag, nvds_tag_len_t length, uint8_t *buf);

#endif //(NVDS_READ_WRITE == 1)

#endif // _NVDS_H_

/// @}
/// @}
