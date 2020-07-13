using System;
using System.Runtime.CompilerServices;

namespace Test
{
	class Test {
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
  		extern static string cout(string msg);

		public static void Main(string[] args)
		{
			cout("Hello, World!");
		}
	}

}