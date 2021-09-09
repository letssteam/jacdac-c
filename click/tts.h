/******************************************************************************
* Title                 :   Text To Speech
* Filename              :   tts.h
* Author                :   MSV
* Origin Date           :   30/01/2016
* Notes                 :
*******************************************************************************/
/**************************CHANGE LIST ****************************************
*
*    Date    Software Version    Initials   Description
*  30/01/16     .1                  MSV     Interface Created.
*
*******************************************************************************/
/**
 * @file tts.h
 * @brief <h3> Text to Speech </h3>
 *
 * @par
 * The device operates as a companion device. It is connected to a host using
 * the SPI. When connected to a host, the operation of the device is completely
 * determined by control and data information provided by the host. Control and
 * data information is transferred to and from device using a defined
 * message protocol.
 *
 * @par
 * The features of the messaging protocol are presented below :
 * - The device is completely driven by the reception of messages from the
 *   host.
 * - The message set is byte oriented, with a fixed length header part,
 *   and variable length payload
 * - The messages follow a request –> response flow. All request messages are
 *   sent by the host processor. All response messages are generated by the
 *   device in response to a request message.
 * - The host processor is required to wait for a response to a request
 *   message, before issuing any subsequent request. Any request message sent
 *   before receiving the previous response will trigger an error state within
 *   the device.
 * - Some messages are termed indication messages. Such messages require no
 *   confirmation from the recipient. Indication messages can only be sent by
 *   the device.
 *
 * @par
 * In general, a response message is sent immediately on receiving its
 * corresponding request message. The sending of a response informs the host
 * that the request has been received but not that it has been actioned.
 * However for some messages, the interval between the sending of a request and
 * receipt of a response can be considerable. In corner cases, the delay
 * between the sending of a request message by a host and the receipt of the
 * corresponding response message can be several hundred milliseconds.
 * Delay between request and response is guaranteed not to exceed
 * MAX_RESPONSE_TIME.
 *
 * @par
 * The device have two working modes - boot and main mode. The device boot mode
 * is entered on hardware reset. The device requires the download of
 * initialisation data in boot mode before it is run in main mode.
 * This initialisation data is contained in the device binary file included
 * with this documentation.
 * @link tts_image.h @endlink
 * In order to enter the main work mode user should call execution of the loaded
 * image
 * @link tts_image_exec @endlink
 * and registration of the host with the device with
 * @link tts_interface_test @endlink.
 * After this steps device enters the main working mode.
 *
 * @par
 * Any request message can be sent at any time after the last response message
 * has been received. If it is inappropriate to send a request message at a
 * certain point, then the device will indicate this with a non-fatal error
 * code in the response message.
 *
 * @par
 * In the normal course of operation, the host should not receive a reserved
 * error from device. Non-fatal errors can be viewed as warnings, from which
 * the system recovers, and have no adverse effect on system behaviour.
 * Non-fatal errors are communicated to the host processor via the appropriate
 * response message, or, in an application specific error indication.
 * A response message that includes an error code indicates that the requested
 * action could not be completed. Fatal errors cannot be recovered by software.
 * The only recovery mechanism is to reset the device. Fore more informations
 * about error codes
 * @link TTS_ERROR_t @endlink.
 *
 * @par
 * All functions inside this module returns recevied response from device so
 * sucessfull execution will return 0 and all other returned values means
 * that function is not executed properly.
 */
#ifndef TEXT_TO_SPEECH_H_
#define TEXT_TO_SPEECH_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include "tts_hw.h"
#include "drv_digital_out.h"
#include "drv_digital_in.h"
#include "drv_i2c_master.h"
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define ISC_VERSION_REQ_BOOT                                    0x0005
#define ISC_VERSION_RESP_BOOT                                   0x0006
#define ISC_BOOT_LOAD_REQ                                       0x1000
#define ISC_BOOT_LOAD_RESP                                      0x1001
#define ISC_BOOT_RUN_REQ                                        0x1002
#define ISC_BOOT_RUN_RESP                                       0x1003

#define ISC_TEST_REQ                                            0x0003
#define ISC_TEST_RESP                                           0x0004
#define ISC_VERSION_REQ_MAIN                                    0x0005
#define ISC_VERSION_RESP_MAIN                                   0x0006
#define ISC_ERROR_IND                                           0x0000
#define ISC_MSG_BLOCKED_RESP                                    0x0007

#define ISC_PMAN_CONFIG_REQ                                     0x0062
#define ISC_PMAN_CONFIG_RESP                                    0x0063
#define ISC_PMAN_STANDBY_ENTRY_REQ                              0x0064
#define ISC_PMAN_STANDBY_ENTRY_RESP                             0x0065
#define ISC_PMAN_STANDBY_EXIT_IND                               0x0066

#define ISC_AUDIO_CONFIG_REQ                                    0x0008
#define ISC_AUDIO_CONFIG_RESP                                   0x0009
#define ISC_AUDIO_VOULME_REQ                                    0x000A
#define ISC_AUDIO_VOLUME_RESP                                   0x000B
#define ISC_AUDIO_MUTE_REQ                                      0x000C
#define ISC_AUDIO_MUTE_RESP                                     0x000D

#define ISC_SPCODEC_CONFIG_REQ                                  0x0056
#define ISC_SPCODEC_CONFIG_RESP                                 0x0057
#define ISC_SPCODEC_START_REQ                                   0x0058
#define ISC_SPCODEC_START_RESP                                  0x0059
#define ISC_SPCODEC_PAUSE_REQ                                   0x005C
#define ISC_SPCODEC_PAUSE_RESP                                  0x005D
#define ISC_SPCODEC_STOP_REQ                                    0x005A
#define ISC_SPCODEC_STOP_RESP                                   0x005B
#define ISC_SPCODEC_READY_IND                                   0x0060
#define ISC_SPCODEC_FINISHED_IND                                0x0061

#define ISC_TTS_CONFIG_REQ                                      0x0012
#define ISC_TTS_CONFIG_RESP                                     0x0013
#define ISC_TTS_SPEAK_REQ                                       0x0014
#define ISC_TTS_SPEAK_RESP                                      0x0015
#define ISC_TTS_READY_IND                                       0x0020
#define ISC_TTS_FINISHED_IND                                    0x0021
#define ISC_TTS_PAUSE_REQ                                       0x0016
#define ISC_TTS_PAUSE_RESP                                      0x0017
#define ISC_TTS_STOP_REQ                                        0x0018
#define ISC_TTS_STOP_RESP                                       0x0019
#define ISC_TTS_UDICT_DATA_REQ                                  0x00CE
#define ISC_TTS_UDICT_DATA_RESP                                 0x00D0
/******************************************************************************
* Configuration Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
/**
 * @enum TTS_ERROR_t
 * @brief <h3> Text to speech errors </h3>
 */
typedef enum
{
    /**
     * No Error */
    NO_ERROR                                                    = 0x0000,
    /**
     * Insufficient system resource to perform request */
    GEN_INSUFF_RESOURCES                                        = 0x4002,
    /**
     * Unrecognised message ID */
    GEN_UNKNOWN_MSG                                             = 0x4003,
    /**
     * Application is not supported by device */
    GEN_APP_NO_SUPPORT                                          = 0x4005,
    /**
     * Invalid audio configuration */
    AUDIO_INVALID_CONF                                          = 0x4020,
    /**
     * Out of range configuration value */
    AUDIO_OUT_OF_RANGE                                          = 0x4021,
    /**
     * Audio message out of sequence */
    AUDIO_OUT_OF_SEQUENCE                                       = 0x4026,
    /**
     * Incompatible routing configuration */
    AUDIO_INVALID_ROUTING_CONF                                  = 0x4028,
    /**
     * Incompatible frequency configuration */
    AUDIO_INVALID_FREQUENCY_CONF                                = 0x4029,
    /**
     * Power nabager invalid configuration */
    PMAN_INVALID_CONF                                           = 0x40C0,
    /**
     * Device is not ready to enter standby mode */
    PMAN_NOT_READY                                              = 0x40C1,
    /**
     * Invalid language */
    TTS_INVALID_LANGUAGE                                        = 0x4040,
    /**
     * Invalid sample rate */
    TTS_INVALID_SAMPLE_RATE                                     = 0x4041,
    /**
     * Invalid voice selection */
    TTS_INVALID_VOICE                                           = 0x4042,
    /**
     * Invalid data source */
    TTS_INVALID_DATA_SRC                                        = 0x4043,
    /**
     * TTS not configured */
    TTS_NOT_CONFIGURED                                          = 0x4044,
    /**
     * TTS not ready */
    TTS_NOT_READY                                               = 0x4045,
    /**
     * Audio output channel open failed */
    TTS_AUDIO_OPEN_FAIL                                         = 0x4046,
    /**
     * Audio output channel close failed */
    TTS_AUDIO_CLOSE_FAIL                                        = 0x4047,
    /**
     * TTS already stopped */
    TTS_STOP_ERR                                                = 0x4048,
    /**
     * Unexpected config request */
    TTS_CONF_REQUEST_ERR                                        = 0x4049,
    /**
     * File already open */
    TTS_OPEN_ERR                                                = 0x404A,
    /**
     * File open failed */
    TTS_OPEN_FAIL                                               = 0x404B,
    /**
     * File close failed */
    TTS_CLOSE_FAIL                                              = 0x404C,
    /**
     * Cannot pause */
    TTS_PAUSE_ERR                                               = 0x404F,
    /**
     * Invalid user dictionary file */
    TTS_DICT_ERR                                                = 0x4052,
    /**
     * Invalid whilst paused */
    TTS_WHIILS_ERR                                              = 0x4053,
    /**
     * Speech codec not configured before starting */
    SPCODEC_START_ERR                                           = 0x4101,
    /**
     * Pause attempted when speech codec is inactive */
    SPCODEC_PAUSE_ERR                                           = 0x4102,
    /**
     * Config attempted when speech codec is active */
    SPCODEC_CONFIG_ERR                                          = 0x4103,
    /**
     * Invalid data source */
    SPCODEC_DATA_SRC_ERR                                        = 0x4104,
    /**
     * Speech codec already paused */
    SPCODEC_ALREADY_PAUSED                                      = 0x4106,
    /**
     * Speech codec already active (not paused) */
    SPCODEC_PAUSE_FAIL                                          = 0x4107,
    /**
     * Invalid codec configuration */
    SPCODEC_INVALID_CONF                                        = 0x4108,
    /**
     * Too much input data*/
    SPCODEC_INPUT_DATA_ERR                                      = 0x4109,
    /**
     * Cannot open audio channel */
    SPCODEC_AUDIO_OPEN_ERR                                      = 0x410B,
    /**
     * Cannot close audio channel */
    SPCODEC_AUDIO_CLOSE_ERR                                     = 0x410C,
    /**
     * Unexpected message */
    FATAL_ERR_UNEXPECTED_MSG                                    = 0x80E0,
    /**
     * Invalid SPCODEC file / CODEC error */
    FATAL_CODEC_ERR_1                                           = 0x8104,
    /**
     * Invalid SPCODEC file / CODEC error */
    FATAL_CODEC_ERR_2                                           = 0x8105,
    /**
     * Invalid SPCODEC file / CODEC error */
    FATAL_CODEC_ERR_3                                           = 0x8106

}TTS_ERROR_t;

/**
 * @enum FF_t
 * @brief <h3> Firmware Features </h3>
 */
typedef enum
{
    /**
     * Text to Speech */
    FF_TTS                                                      = 0x0001,
    /**
     * ADPCM */
    FF_ADPCM                                                    = 0x0010,
    /**
     * GPIO */
    FF_GPIO                                                     = 0x0100

}FF_t;

/**
 * @enum EFF_t
 * @brief <h3> Extended Firmware Features </h3>
 *
 * @par
 * TTS language support.
 */
typedef enum
{
    /**
     * US English */
    EFF_US                                                      = 0x01,
    /**
     * German */
    EFF_D                                                       = 0x02,
    /**
     * Castilian Spanish */
    EFF_CS                                                      = 0x04,
    /**
     * French */
    EFF_F                                                       = 0x08,
    /**
     * UK English */
    EFF_UK                                                      = 0x10,
    /**
     * Latin Spanish */
    EFF_LS                                                      = 0x20

}EFF_t;

/**
 * @enum ASR_t
 * @brief <h3> Audio Sample Rate </h3>
 *
 * @par
 * Audio sample rate configuration.Used in audio configuration.
 *
 * @note
 * If desired, the ASR_AUTO setting may be used for ADPCM file playback. In
 * this case the sample rate used is the sample rate that was specified by the
 * encoder and stored in the encoded ADPCM file header.
 *
 * @note
 * ASR_11KHZ is reserved for use with TTS synthesis. All other sample rates
 * are reserved for use with ADPCM.
 */
typedef enum
{
    /**
     * 8 kHz ( usable ) */
    ASR_8KHZ                                                    = 0x00,
    /**
     * 11.025 kHz ( usable ) */
    ASR_11KHZ                                                   = 0x01,
    /**
     * 12 kHz ( reserved ) */
    ASR_12KHZ                                                   = 0x02,
    /**
     * 16 kHz ( usable ) */
    ASR_16KHZ                                                   = 0x03,
    /**
     * 22.05 kHz ( reserved ) */
    ASR_22KHZ                                                   = 0x04,
    /**
     * 24 kHz ( reserved ) */
    ASR_24KHZ                                                   = 0x05,
    /**
     * 32 kHz ( reserved ) */
    ASR_32KHZ                                                   = 0x06,
    /**
     * 44.1 kHz ( reserved ) */
    ASR_44KHZ                                                   = 0x07,
    /**
     * 48 kHz ( reserved ) */
    ASR_48KHZ                                                   = 0x08,
    /**
     * Set by application */
    ASR_AUTO                                                    = 0x09

}ASR_t;

/**
 * @enum TTS_VOICE_t
 * @brief <h3> Voice Configuration </h3>
 */
typedef enum
{
    /**
     * UK English */
    TTSV_US                                                     = 0x00,
    /**
     * Castilian Spanish */
    TTSV_CS                                                     = 0x01,
    /**
     * Latin Spanish */
    TTSV_LS                                                     = 0x04

}TTSV_t;

/**
 * @struct VER_t
 * @brief <h3> Device version </h3>
 *
 * @note
 * For the hardware version, the first number refers to the hardware  platform,
 * and the second number refers to the ROM revision number.
 *
 * @note
 * The firmware versions are defined as version X.Y.Z. where X is the major
 * release number, Y is the minor release number ( typically associated with a
 * functionality enhancement ), and Z is an incremental ( bug fix ) release.
 */
typedef struct
{
    /**
     * Hardware version*/
    char            hwver[ 5 ];
    /**
     * Firmware version*/
    char            fwver[ 8 ];
    /**
     * Firmware features*/
    FF_t            fwf;
    /**
     * Firmware extended features*/
    EFF_t           fwef;

}VER_t;

/**
 * @struct ACONF_t
 * @brief <h3> Audio Configuration </h3>
 *
 * @note
 * If DAC permanently on is set and minimum delay time is required before
 * audio output after receiving a data bearing message, an asr must be
 * selected. It must not be set to ASR_AUTO.
 *
 */
typedef struct
{
    /**
     * Audio stereo ( 0x00 ) */
    bool            as;
    /**
     * Audio gain value ( 0x00 ~ 0x43 ) */
    uint8_t         ag;
    /**
     * Audio amplefier ( 0x00 ) */
    bool            amp;
    /**
     * Audio sample rate ( 0x00 / 0x01 / 0x03 / 0x09 ) */
    ASR_t           asr;
    /**
     * Audio routing ( 0x00 ) */
    uint8_t         ar;
    /**
     * Audio tone control ( 0x00 ) */
    uint8_t         atc;
    /**
     * Audio clock source ( 0x00 ) */
    bool            acs;
    /**
     * DAC control ( 0x00 / 0x01 ) */
    bool            dc;

}ACONF_t;

/**
 * @struct TTS_CONFIG_t
 * @brief <h3> Text To Speech Configuration </h3>
 *
 * @par
 * TTS configuration can only be used when the TTS system is inactive.
 *
 * @note
 * The valid range for speaking rate is 0x004B to 0x0258. A suitable default
 * value is 200 words/min ( 0x00C8 ).
 */
typedef struct
{
    /**
     * Sample rate ( 0x01 ) */
    uint8_t         sr;
    /**
     * Voice selection ( 0x01 ~ 0x09 ) */
    uint8_t         voice;
    /**
     * Epson parse ( true / false ) */
    bool            ep;
    /**
     * TTS language ( 0x01 / 0x01 / 0x04 ) */
    TTSV_t          lang;
    /**
     * Speaking rate in words per minute */
    uint8_t         sr_wpm_lsb;
    uint8_t         sr_wpm_msb;
    /**
     * Data Source ( 0x00 ) */
    uint8_t         ds;
    /**
     * Reserved ( 0x00 ) */
    uint8_t         res;

}TTSCONF_t;

/**
 * @struct PMANCONF_t
 * @brief <h3> Power Manager Configuration </h3>
 */
typedef struct
{
    /**
     * Audio mode ( 0x01 ) */
    uint8_t         am_lsb;
    uint8_t         am_msb;
    /**
     * SPI bit clock frequency ( 0x01 ) */
    uint8_t         spi_clk;
    /**
     * Padding byte */
    uint8_t         pad;

}PMANCONF_t;

/******************************************************************************
* Variables
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief <h3> Text to Speech Initialization </h3>
 *
 * @par
 * Sets all needed internal parameters to default. This function must be called
 * before any other function.
 */
void tts_init( void );

/**
 * @brief <h3> Message Block Callback </h3>
 *
 * @par
 * Callback function must be made by user and will be called
 * every time when message block response is received.
 * Message block is sent in response to a request that has been blocked by
 * the device system controller. Request messages are blocked usually when
 * the system controller identifies that there is insufficient system
 * resource to service the request.
 *
 * @note
 * All request messages are blocked if the interface has not been
 * registered.
 *
 * @par
 * <h4> Example :</h4>
 *
 * @code
 * // Declaration of callback for blocked messages
 * void msg_blk( uint16_t *req, uint16_t *err )
 * {
 *     char txt[ 15 ];
 *
 *     UART_Write_Text( " Message blocked : " );
 *     sprinti( txt, "%x\r\n", *req );
 *     UART_Write_Text( txt );
 *     sprinti( txt, "%x\r\n", *err );
 *     UART_Write_Text( txt );
 * }
 *
 * // Setting up
 * tts_msg_block_callback( msg_blk );
 * @endcode
 *
 */
void tts_msg_block_callback( void( *msg_blk_ptr )( uint16_t *req_ptr,
                                                   uint16_t *err_ptr ) );
/**
 * @brief <h3> Fatal Error Callback </h3>
 *
 * @par
 * This function adds the callback function made by user that will be called
 * every time when fatal error indication is received so user can handle
 * fatal error exception. Once fatal error is received user should
 * restart the module.
 *
 * @par
 * <h4> Example :</h4>
 *
 * @code
 * // Declaration of callback for fatal errors
 * void fatal_err( uint16_t *err )
 * {
 *     UART_Write_Text( "Fatal Error Occured - TTS restart..." );
 *     // Reinitialization of TTS
 *     tts_init();
 *     // Reset the callback
 *     tts_fatal_err_callback( fatal_err );
 * }
 *
 * // Setting up
 * tts_fatal_err_callback( fatal_err );
 * @endcode
 */
void tts_fatal_err_callback( void( *fatal_err_ptr )( uint16_t *err_ptr ) );

/**
 * @brief <h3> Non Fatal Error </h3>
 *
 * @par
 * This function adds the callback function made by user that will be called
 * every time when non fatal error inidication is received.
 *
 * @par
 * <h4> Example :</h4>
 *
 * @code
 * // Declaration of callback for error occurence
 * void err_detected( uint16_t *err )
 * {
 *     char txt[ 15 ];
 *
 *     UART_Write_Text( " Error occured : " );
 *     sprinti( txt, "%x\r\n", *err );
 *     UART_Write_Text( txt );
 * }
 *
 * // Setting up
 * tts_err_callback( err_detected );
 * @endcode
 */
void tts_err_callback( void( *error_ptr )( uint16_t *err_ptr ) );

/**
 * @brief <h3> TTS Hardware Mute </h3>
 *
 * @par
 * Mute using MUT pin
 */
void tts_mute( void );

/**
 * @brief <h3> TTS Hardware Unmute </h3>
 *
 * @par
 * Unmute using MUT pin
 */
void tts_unmute( void );

/**
 * @brief <h3> Default setup </h3>
 *
 * @par
 * Should be used after the @link tts_init @endlink to upload the init data
 * and configure the click board in the most common manner.
 */
void tts_setup( void );

/**
 * @brief <h3> Requests Version in Boot Mode </h3>
 *
 * @par
 * Used to request information about the chip hardware version.
 * This request should be performed after the power on or restart
 * routine. The functionality to wait
 * @link RESET_TO_BOOT_TIME @endlink
 * is already implemented into the restart function and don't care
 * about that timing. This is the simpliest way to check does device is
 * powered on and to check the basic functionality of the device itself.
 * For detailed information about version of the device you should use
 * @link tts_version_main @endlink . For more information
 * @link VER_t @endlink .
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * VER_t boot_ver;
 *
 * if( !tts_version_boot( boot_ver ) )
 * {
 *     UART_Write_Text( boot_ver.hwver );
 * }
 * @endcode
 */
uint16_t tts_version_boot( VER_t *version );

/**
 * @brief <h3> Loads Code Image into the SRAM </h3>
 *
 * @par
 * This request must be pefrormed after the power on or restart
 * routine and before the entering to main mode.
 *
 * @param[in] image - valid image
 * @param[in] count - image length in bytes
 *
 * @note Keep on mind that because of large image size this loadnig can take
 * few seconds, depends on SPI bus speed.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_image_load( TTS_INIT_DATA, sizeof( TTS_INIT_DATA ) ) )
 *     UART_Write_Text( "Image loading done successfully");
 * else
 *     UART_Write_Text( "Image loading error");
 * @endcode
 */
uint16_t tts_image_load( const uint8_t *image,
                         uint16_t count );

/**
 * @brief <h3> Starts Executing Image and Enters the Main Mode </h3>
 *
 * @par
 * This request enters from boot to main mode after completion of loading the
 * initialisation data. This function is used to switch to running in main
 * mode. The functionality to wait
 * @link
 * BOOT_TO_MAIN_MODE
 * @endlink
 * is already implemented inside the restart function so don't care
 * about that timing.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_image_exec() )
        UART_Write_Text( "Main mode entered successfully" );
 * @endcode
 */
uint16_t tts_image_exec( void );

/**
 * @brief <h3> Test & Register Host Hardware Interface </h3>
 *
 * @par
 * This request must be called after the entering the main mode because it
 * register the host with the device ( S1V30120 ). If request is performed
 * without errors that indicates the device is operating correctly and host
 * registration was successful.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_interface_test() )
 *     UART_Write_Text( "Interface test - success" );
 * @endcode
 */
uint16_t tts_interface_test( void );

/**
 * @brief <h3> Version & Support Info in Main Mode </h3>
 *
 * @par
 * Used to request information about the hardware version, firmware
 * version and supported firmware functionality. Returns informations
 * about current system version, more info
 * @link VER_t @endlink
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * VER_t current_version;
 *
 * if ( !tts_version_main( &current_version ) )
 * {
 *     UART_Write_Text( current_version.hwver );
 *     UART_Write_Text( current_version.fwver );
 * }
 * @endcode
 */
uint16_t tts_version_main( VER_t *buffer );

/**
 * @brief <h3> Power Manager Configuration </h3>
 *
 * @par
 * Request to configure power manager with default parameters.
 *
 * @note All of parameters are configured automaticly by device
 * and this request also doesnt have any parameters yet.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_power_default_config() )
 *     UART_Write_Text( "Power configuration done !" );
 * @endcode
 */
uint16_t tts_power_default_config( void );

/**
 * @brief <h3> Entry to standby mode </h3>
 *
 * @par
 * Request can be used to reach the lowest power mode.
 * Most power management on the device is performed automatically by the
 * device without intervention from the host.
 * However, to reach the lowest power mode, Standby Mode, the host has to
 * request entry to this mode. Device can leave this mode only with
 * @link tts_standby_exit @endlink.
 * Time needed to enter the standby mode
 * @link STBY_MODE_ENTERY @endlink
 * is already implemented and you don't have to care about that.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_standby_enter() )
 *     UART_Write_Text( "Standby mode entered !" );
 * @endcode
 */
uint16_t tts_standby_enter( void );

/**
 * @brief <h3> Standby Mode Exit </h3>
 *
 * @par
 * Request wakes up the device from standby mode. This is only request
 * that device can performe whhile it is in standby mode.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_standby_exit() )
 *     UART_Write_Text( "Standby mode exited !" );
 * @endcode
 */
uint16_t tts_standby_exit( void );

/**
 * @brief <h3> Configure Audio Output </h3>
 *
 * @par
 * Audio configuration is required before starting speech/audio playback.
 * Request sends default audio configuration to device.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_audio_default_config() )
 *     UART_Write_Text( "Audio configuration done !" );
 * @endcode
 */
uint16_t tts_audio_default_config( void );

/**
 * @brief <h3> Sets the audio output configuration. </h3>
 *
 * @par
 * Audio configuration is required before starting speech/audio playback.
 *
 * @param[in] audio_gain ( -49 ~ 19 )
 * @param[in] sample_rate
 * @param[in] dac_control ( true / false )
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_audio_config( 10, ASR_11KHZ, true ) )
 *     UART_Write_Text( "Advanced audio configuration done !" );
 * @endcode
 */
uint16_t tts_audio_config( int8_t audio_gain,
                           ASR_t sample_rate,
                           bool dac_control );

/**
 * @brief <h3> Set volume ( Analogue Gain ) </h3>
 *
 * @par
 * Request changes the output audio volume and sends it to the device.
 * Value is gain increment / decrement in dB.
 *
 * @param[in] audio_gain
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_volume_set( 0 ) )
 *     UART1_Write_Text( "Volume setting done !" );
 * @endcode
 */
uint16_t tts_volume_set( int16_t gain );

/**
 * @brief <h3> Mute Audio Output </h3>
 *
 * @par
 * Request mutes the audio output immediately.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_audio_mute() )
 *     UART1_Write_Text( "Audio muted !" );
 * @endcode
 */
uint16_t tts_audio_mute( void );

/**
 * @brief <h3> Unmute Audio Output </h3>
 *
 * @par
 * Request mutes the audio output immediately.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_audio_unmute() )
 *     UART1_Write_Text( "Audio unmuted !" );
 * @endcode
 */
uint16_t tts_audio_unmute( void );

/**
 * @brief <h3> Configure Speech Codec </h3>
 *
 * @par
 * The Speech Codec application supports Voice Playback (Decode) using an
 * ADPCM codec. This is request to configure the codec and can only be used
 * when the SPCODEC is inactive. Currently all parameters are default so there
 * is no setable parameters in this request.
 */
uint16_t tts_codec_configure( void );

/**
 * @brief <h3> Start Speech Codec </h3>
 *
 * @par
 * Request is used to transfer data to the speech decoder.
 */
uint16_t tts_codec_start( uint8_t *codec_data,
                          uint16_t count );

/**
 * @brief <h3> Pause Speech Codec Audio Input / Output </h3>
 *
 * @par
 * Request to pause the speech decoders to processing speech data.
 */
uint16_t tts_codec_pause( void );

/**
 * @brief <h3> Unpause Speech Codec Audio Input / Output </h3>
 *
 * @par
 * Request to unpause the speech decoders to processing speech data.
 */
uint16_t tts_codec_unpause( void );

/**
 * @brief <h3> Stop Speech Codec Immediately </h3>
 *
 * @par
 * Request to terminate the decode of speech data immediately. Audio data
 * already decoded will be processed before termination. If the SPCODEC is
 * reset, system resources used by the SPCODEC are freed, and all configuration
 * data is lost ( the SPCODEC is finalised ).
 *
 * @param[in] reset
 */
uint16_t tts_codec_stop( bool reset );

/**
 * @brief <h3> Configure the TTS </h3>
 *
 * @par
 * Request sends the default configuration to the device and can be used when
 * the TTS system is inactive.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_default_config() )
 *     UART1_Write_Text( "TTS configured !\" );
 * @endcode
 */
uint16_t tts_default_config( void );

/**
 * @brief <h3> Sets the text to speech configuration </h3>
 *
 * @par
 * Request sends the user provided configuration to the device and can be used
 * when the TTS system is inactive.
 *
 * @param[in] voice_type ( 0 ~ 9 )
 * @param[in] epson_parse ( true / false )
 * @param[in] language
 * @param[in] speaking_rate ( 0x004A ~ 0x0259 )
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_config( 0x01, false, TTSV_US, 0x0200 ) )
 *     UART_Write_Text( "TTS advanced configuration done !" );
 * @endcode
 */
uint16_t tts_config( uint8_t voice_type,
                     bool epson_parse,
                     TTSV_t language,
                     uint16_t speaking_rate );

/**
 * @brief <h3> Start the TTS ( Optionally Attach Data ) </h3>
 *
 * @par
 * Request sends the text data to the device. User can't send more data
 * while the response indication that device is ready for more data is not
 * received.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_speak( "Hello world" ) )
 *     UART1_Write_Text( "Speak done !" );
 * @endcode
 */
uint16_t tts_speak( char *word );

/**
 * @brief <h3> Pause the TTS </h3>
 *
 * @par
 * Request will stop or restart the speech synthesiser text proccessing.
 * Time must be allowed for the audio output buffers to drain before the pause
 * can take effect.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_pause() )
 *     UART_Write_Text( "Paused !" );
 * @endcode
 */
uint16_t tts_pause( void );

/**
 * @brief <h3> Unpause the TTS </h3>
 *
 * @par
 * Request will resume the speech synthesiser.
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_unpause() )
 *     UART_Write_Text( "Unpaused !" );
 * @endcode
 */
uint16_t tts_unpause( void );

/**
 * @brief <h3> Stop TTS Immediately </h3>
 *
 * @par
 * Request that should be executed to stop text sythesis in order to free
 * system resources associated with TTS. Device can be reseted after the this
 * request and in that case all configuration data is lost.
 *
 * @param[in] reset ( true / false )
 *
 * @par
 * <h4> Example : </h4>
 *
 * @code
 * if( !tts_stop() )
 *     UART_Write_Text( "Stoped !" );
 * @endcode
 */
uint16_t tts_stop( bool reset );

/**
 * @brief <h3> Send User Dictionary Data to the Device </h3>
 *
 * @par
 * Request sends user dictionary data. User dictionary data is transferred in a
 * single message. Erase parameter is used to erase any previously transferred
 * user dictionary data. Udict data must be valid and counter is size of data
 * in bytes.
 *
 * @param[in] erase ( true / false )
 * @param[in] udict_data
 * @param[in] count
 */
uint16_t tts_user_dict( bool erase,
                        uint8_t *udict_data,
                        uint16_t count );

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TEXT_TO_SPEECH_H_
/*** End of File **************************************************************/