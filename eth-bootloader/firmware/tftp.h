// TFTP Opcode values from RFC 1350
//
#define TFTP_OPCODE_RRQ   1
#define TFTP_OPCODE_WRQ   2
#define TFTP_OPCODE_DATA  3
#define TFTP_OPCODE_ACK   4
#define TFTP_OPCODE_ERROR 5

// TFTP Error codes from RFC 1350
//
#define TFTP_ERROR_UNDEF        0
#define TFTP_ERROR_NOT_FOUND    1
#define TFTP_ERROR_ACCESS       2
#define TFTP_ERROR_FULL         3
#define TFTP_ERROR_ILLEGAL_OP   4
#define TFTP_ERROR_UNKNOWN_XFER 4
#define TFTP_ERROR_EXISTS       6
#define TFTP_ERROR_NO_SUCH_USER 7


#define TFTP_PORT 69

void tftpInit();
uint8_t tftpPoll();
