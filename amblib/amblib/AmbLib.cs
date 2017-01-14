using System;
using System.Collections.Generic;
using System.Text;

namespace Ambiesoft
{
    public class AmbLib
    {
        public static bool IsFileNamable(string fn)
        {
            if (string.IsNullOrEmpty(fn))
                return false;

            if (fn.EndsWith(".") || fn.EndsWith(" "))
                return false;

            foreach (char c in fn)
            {
                if (c == '<' ||
                     c == '>' ||
                     c == ':' ||
                     c == '\"' ||
                     c == '/' ||
                     c == '\\' ||
                     c == '|' ||
                     c == '?' ||
                     c == '*')
                {
                    return false;
                }

                if (0 <= c && c <= 31)
                {
                    return false;
                }
            }

            string lfn = fn.ToUpper();
            int lp = lfn.LastIndexOf('.');
            if (lp != -1)
            {
                lfn = lfn.Substring(0, lp);
            }

            if (
                lfn == "CON" ||
                lfn == "PRN" ||
                lfn == "AUX" ||
                lfn == "NUL" ||
                lfn == "COM1" ||
                lfn == "COM2" ||
                lfn == "COM3" ||
                lfn == "COM4" ||
                lfn == "COM5" ||
                lfn == "COM6" ||
                lfn == "COM7" ||
                lfn == "COM8" ||
                lfn == "COM9" ||
                lfn == "LPT1" ||
                lfn == "LPT2" ||
                lfn == "LPT3" ||
                lfn == "LPT4" ||
                lfn == "LPT5" ||
                lfn == "LPT6" ||
                lfn == "LPT7" ||
                lfn == "LPT8" ||
                lfn == "LPT9"
                )
            {
                return false;
            }

            return true;
        }
    }
}
