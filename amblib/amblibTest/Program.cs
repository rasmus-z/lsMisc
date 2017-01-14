using System;
using System.Collections.Generic;
using System.Text;
using Ambiesoft;
using System.Diagnostics;

namespace amblibTest
{
    class Program
    {
        static void verify(bool b)
        {
            if (!b)
            {
                Console.WriteLine("BAD");
            }
        }

        static void Main(string[] args)
        {
            verify(AmbLib.IsFileNamable("aaa"));
            verify(AmbLib.IsFileNamable("aaa.txt"));
            verify(AmbLib.IsFileNamable("Ç†Ç†Ç†Ç†ÅB.aaa"));
            verify(!AmbLib.IsFileNamable("afeafe "));
            verify(AmbLib.IsFileNamable(".aaa"));
            verify(!AmbLib.IsFileNamable("aaa "));
            verify(!AmbLib.IsFileNamable("fnjsa<"));
            verify(AmbLib.IsFileNamable("aaan,.ji.jjjjjjjjjjjjjjjj"));
            verify(!AmbLib.IsFileNamable("lpt1.txt"));
            verify(!AmbLib.IsFileNamable("aux"));
        }
    }
}
