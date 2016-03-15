#include <stdio.h>

void func (void)
{
	{
		static int  x;
		{
			static int  x;
			{
				static int  x;

			}

		}
	}
}

int main()
{
 func();
}
