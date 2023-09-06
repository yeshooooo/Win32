// 内存操作函数memset，memcpy的故事
// https://www.cctry.com/forum.php?mod=viewthread&tid=298054&highlight=%C4%DA%B4%E6%B2%D9%D7%F7%BA%AF%CA%FDmemset%A3%ACmemcpy%B5%C4%B9%CA%CA%C2
#include <stdio.h>
#include <vcruntime_string.h>


int main()
{
	char src_rgba[16] = {
		0x0f, 0x0e, 0x0d, 0x0c,
		0x0b, 0x0a, 0x09, 0x08,
		0x07, 0x06, 0x05, 0x04,
		0x03, 0x02, 0x01, 0x0
	};
	char dst_rgba[16];

	// 1.初始化为黑色
	memset(dst_rgba, 0x0, 16);


}