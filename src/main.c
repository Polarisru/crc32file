#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include "crc.h"

#define DO_CRC32

#ifdef DO_CRC16
  #define CRC_INIT    0xffff
  #define CRC_SIZE    sizeof(uint16_t)
  #define CRC_TYPE    *(uint16_t*)
  #define CRC_FORMAT  "0x%04X"
  #define CRC_FUNC    CRC16_Calc
#endif // DO_CRC16
#ifdef DO_CRC32
  #define CRC_INIT    0xffffffff
  #define CRC_SIZE    sizeof(uint32_t)
  #define CRC_TYPE    *(uint32_t*)
  #define CRC_FORMAT  "0x%08X"
  #define CRC_FUNC    CRC32_Calc
#endif // DO_CRC32

uint8_t buf[1024];

void print_error(char *msg, int err_code)
{
  printf("ERROR: ");
  printf(msg);
  printf("\n");
  exit(err_code);
}

int main(int argc, char *argv[])
{
	int n;
	FILE *fd;
	#ifdef DO_CRC16
  uint16_t crc, crc2;
  #endif
	#ifdef DO_CRC32
  uint32_t crc, crc2;
  #endif
  uint32_t size, pos;

	if (argc != 2)
  {
    printf("USAGE: crcfile FILENAME\n");
    return 0;
  }
	fd = fopen(argv[1], "rb");
	if (fd == NULL)
    print_error("Can't open file for reading!", 1);

  /**< Get file size */
  fseek(fd, 0L, SEEK_END);
  size = ftell(fd);
  pos = 0;
  n = 0;

  /**< Move to start of the file */
  fseek(fd, 0L, SEEK_SET);

	crc = CRC_INIT;
	crc2 = CRC_INIT;

  /**< Reading binary file in 1024-bytes blocks */
	while (pos < size)
  {
    if ((size - pos) >= 1024)
      n = fread(buf, sizeof(uint8_t), 1024, fd);
    else
      n = fread(buf, sizeof(uint8_t), size - pos, fd);
    if (n == 0)
      print_error("Can't read data from file!", 2);
		crc = CRC_FUNC(buf, n, crc);
		pos += n;
		if (pos < size)
      crc2 = CRC_FUNC(buf, n, crc2);
    else
      crc2 = CRC_FUNC(buf, n - CRC_SIZE, crc2);
	}
	fclose(fd);
	#ifdef DO_CRC32
	/**< Have to XOR the output for CRC32 */
	crc ^= 0xFFFFFFFF;
	crc2 ^= 0xFFFFFFFF;
	#endif // DO_CRC32
	if (crc2 == CRC_TYPE&buf[n - CRC_SIZE])
  {
		printf("WARNING: CRC has already been appended\n");
		return 0;
	}

	/**< Write CRC to the end of the file */
	fd = fopen(argv[1], "a+b");
	if (fd == NULL)
    print_error("Can't open file for writing!", 3);
	fwrite(&crc, sizeof(uint8_t), CRC_SIZE, fd);
	fclose(fd);

	printf("File size: %d bytes. Appended CRC: "CRC_FORMAT"\n", size, crc);
	return 0;
}
