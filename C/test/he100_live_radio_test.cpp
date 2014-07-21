#include "gtest/gtest.h"
#include "../inc/SC_he100.h"
#include "../inc/timer.h"
#include "../inc/fletcher.h"
#include <Date.h>
#include <SpaceDecl.h>

#define MAX_TESTED_PAYLOAD 220

class Helium_100_Live_Radio_Test : public ::testing::Test
{
    public:
      int fdin;
    protected:
      virtual void SetUp() {
        fdin = HE100_openPort(); 
        if (fdin==-1) exit(EXIT_FAILURE);
      }
      virtual void TearDown() {
        HE100_closePort(fdin);
      }
      size_t z; // assert loop index
};

// will be tested in following tests, but isolate some
// test null bytes, passing wrong length, etc
//struct fletcher_checksum fletcher_checksum16 (char *data, size_t bytes);

// Pass the function some data and check against expected result
unsigned char * HE100_prepareTransmission (unsigned char *payload, size_t length, unsigned char *command);

// Test writing to the helium device
TEST_F(Helium_100_Live_Radio_Test, GoodWrite)
{
    unsigned char write_test[8] = {0x48,0x65,0x10,0x01,0x00,0x00,0x11,0x43}; // 8
    int write_result = HE100_write(fdin, write_test, 8);

    ASSERT_EQ(
        0,
        write_result
    );
    // TODO READ THE ACTUAL BYTE SEQUENCE RETURNED
}

// Test NOOP
TEST_F(Helium_100_Live_Radio_Test, NOOP)
{
    int noop_result = HE100_NOOP(fdin);

    ASSERT_EQ(
        0,
        noop_result
    );
    // TODO READ THE ACTUAL BYTE SEQUENCE RETURNED
}

// Test transmitData
TEST_F(Helium_100_Live_Radio_Test, TransmitData)
{
    unsigned char payload[5] = {0x48,0x65,0x6c,0x6c,0x6f};
    int transmit_result = HE100_transmitData(fdin,payload,5);

    ASSERT_EQ(
        0,
        transmit_result
    );
    // TODO READ THE ACTUAL BYTE SEQUENCE RETURNED
}

// Test setBeaconMessage
TEST_F(Helium_100_Live_Radio_Test, SetBeaconMessage)
{
    unsigned char payload[7] = {0x62,0x65,0x61,0x63,0x6F,0x6E,0x0A};
    int beacon_message_result = HE100_setBeaconMessage(fdin,payload,7);

    ASSERT_EQ(
        0,
        beacon_message_result 
    );
    // TODO READ THE ACTUAL BYTE SEQUENCE RETURNED
}

// Test setBeaconInterval
TEST_F(Helium_100_Live_Radio_Test, SetBeaconInterval)
{
    int interval = 3;
    int beacon_interval_result = HE100_setBeaconInterval(fdin, interval);

    ASSERT_EQ(
        0,
        beacon_interval_result 
    );
    // TODO READ THE ACTUAL BYTE SEQUENCE RETURNED
}

// test passing invalid PA level
TEST_F(Helium_100_Live_Radio_Test, InvalidPALevel)
{
    int ipl_actual_value = HE100_fastSetPA (fdin, 300);
    ASSERT_EQ(
        1,
        ipl_actual_value
    );

    ipl_actual_value = HE100_fastSetPA (fdin, -3);
    ASSERT_EQ(
        1,
        ipl_actual_value
    );
}

// Test fastSetPA
TEST_F(Helium_100_Live_Radio_Test, FastSetPA)
{
    int fast_set_pa_result = HE100_fastSetPA(fdin,7);
    ASSERT_EQ(
        0,
        fast_set_pa_result 
    );

    fast_set_pa_result = HE100_fastSetPA(fdin,0);
    ASSERT_EQ(
        0,
        fast_set_pa_result 
    );
    // TODO READ THE ACTUAL BYTE SEQUENCE RETURNED
}


// Test softReset
TEST_F(Helium_100_Live_Radio_Test, SoftReset)
{
    int soft_reset_result = HE100_softReset(fdin);

    ASSERT_EQ(
        0,
        soft_reset_result
    );
    // TODO READ THE ACTUAL BYTE SEQUENCE RETURNED
}

// test readFirmwareRevision
TEST_F(Helium_100_Live_Radio_Test, ReadFirmwareRevision)
{
    int read_firmware_result = HE100_readFirmwareRevision(fdin);

    ASSERT_EQ(
        0,
        read_firmware_result 
    );
    // TODO READ THE ACTUAL BYTE SEQUENCE RETURNED
}

TEST_F(Helium_100_Live_Radio_Test, TestMaxLength)
{
    unsigned char data[256] = 
        {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
    unsigned char test_data[255];
    memcpy (test_data, data, MAX_TESTED_PAYLOAD);
    int transmit_result = HE100_transmitData(fdin,test_data,MAX_TESTED_PAYLOAD);
    ASSERT_EQ(
        0,
        transmit_result
    );
    memcpy (test_data, data, MAX_TESTED_PAYLOAD+10);
    transmit_result = HE100_transmitData(fdin,test_data,MAX_TESTED_PAYLOAD);
    ASSERT_EQ(
        0,
        transmit_result
    );
}

TEST_F(Helium_100_Live_Radio_Test, TestAllBytes)
{
     unsigned char data[256] = 
        {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
    unsigned char test_data[MAX_TESTED_PAYLOAD];
    memcpy (test_data, data, MAX_TESTED_PAYLOAD);
    int transmit_result = HE100_transmitData(fdin,test_data,MAX_TESTED_PAYLOAD);

    ASSERT_EQ(
        0,
        transmit_result
    );
    memcpy (test_data, data+MAX_TESTED_PAYLOAD, 255-MAX_TESTED_PAYLOAD);
    transmit_result = HE100_transmitData(fdin,test_data,MAX_TESTED_PAYLOAD);
    ASSERT_EQ(
        0,
        transmit_result
    );
}
/*
Send Beacon Data
486510100100217231313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131B4E8
He[16][16][1][0]!r1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111[180][232]
*/