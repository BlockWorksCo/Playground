


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*
Data ::= CHOICE
{
  null-data            [0]   IMPLICIT NULL,
  array                [1]   IMPLICIT SEQUENCE OF Data,
  structure            [2]   IMPLICIT SEQUENCE OF Data,
  bool                 [3]   IMPLICIT BOOLEAN ,
  bit-string           [4]   IMPLICIT BIT STRING ,
  double-long          [5]   IMPLICIT Integer32,
  double-long-unsigned [6]   IMPLICIT Unsigned32,
  octet-string         [9]   IMPLICIT OCTET STRING ,
  visible-string       [10]  IMPLICIT VisibleString,
  bcd                  [13]  IMPLICIT Integer8,
  integer              [15]  IMPLICIT Integer8,
  long-integer         [16]  IMPLICIT Integer16,
  unsigned             [17]  IMPLICIT Unsigned8,
  long-unsigned        [18]  IMPLICIT Unsigned16,
  compact-array        [19]  IMPLICIT SEQUENCE
  {
    contents-description [0] TypeDescription,
    array-contents       [1] IMPLICIT OCTET STRING
  },
  long64               [20]  IMPLICIT Integer64,
  long64-unsigned      [21]  IMPLICIT Unsigned64,
  enumerate            [22]  IMPLICIT Enum,
  float32              [23]  IMPLICIT OCTET STRING (SIZE(4)),
  float64              [24]  IMPLICIT OCTET STRING (SIZE(8)),
  date-time            [25]  IMPLICIT OCTET STRING (SIZE(12)),
  date                 [26]  IMPLICIT OCTET STRING (SIZE(5)),
  time                 [27]  IMPLICIT OCTET STRING (SIZE(4)),
  dont-care            [255] IMPLICIT NULL
}
*/


typedef enum
{
  null_data           = 0,
  array               = 1,
  structure           = 2,
  boolean             = 3,
  bit_string          = 4,
  integer32           = 5,
  unsigned32          = 6,
  octet_string        = 9,
  visible_string      = 10,
  bcd                 = 13,
  integer8            = 15,
  integer16           = 16,
  unsigned8           = 17,
  unsigned16          = 18,
  compact_array       = 19,
  integer64           = 20,
  unsigned64          = 21,
  enumerate           = 22,
  float32             = 23,
  float64             = 24,
  date_time           = 25,
  date                = 26,
  time                = 27,
  dont_care           = 255

} PrimitiveTypeTag;


typedef struct
{
    uint8_t type;
    union
    {
        uint8_t     null_data;
        uint8_t     array;
        uint8_t     structure;
        uint8_t     boolean;
        uint8_t     bit_string[1];
        int32_t     integer32;
        uint32_t    unsigned32;
        uint8_t     octet_string[1];
        uint8_t     visible_string[1];
        uint8_t     bcd;
        uint8_t     integer8;
        int16_t     integer16;
        uint8_t     unsigned8;
        uint16_t    unsigned16;
        uint8_t     compact_array;
        int64_t     integer64;
        uint64_t    unsigned64;
        uint8_t     enumerate;
        float       float32;
        double      float64;
        uint8_t     date_time;
        uint8_t     date;
        uint8_t     time;
        uint8_t     dont_care;

    } value;

} __attribute__((packed)) Field;


typedef struct
{
    uint8_t tag;
    uint8_t numberOfElements;
    Field   fields[1];

} __attribute__((packed)) Structure;


int main()
{
    //uint8_t message[]   = {0x06, 0x01,0x02,0x03,0x04};
    //Field*  field       = (void*)&message;

    uint8_t message[]   = {0x02,0x02, 0x06, 0x01,0x02,0x03,0x04, 0x12, 0x05,0x06};
    Structure*  structure   = (void*)&message[0];

    printf("%d %04x\n",field->type, field->value.unsigned32);

    return 0;
}

