
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace Sim900Plugin
{
    public static class MyStringExtensions
    {
        public static bool Like(this string toSearch, string toFind)
        {

            if (string.IsNullOrEmpty(toFind))
                return true;
            else
            {
                toFind = toFind.Replace("*", "%").ToUpper();
                toSearch = toSearch.ToUpper();
                return new Regex(@"\A" + new Regex(@"\.|\$|\^|\{|\[|\(|\||\)|\*|\+|\?|\\").Replace(toFind, ch => @"\" + ch).Replace('_', '.').Replace("%", ".*") + @"\z", RegexOptions.Singleline).IsMatch(toSearch);
            }
        }

        public static string [] ParseCommand(this string command, string begin)
        { 
                return command.Substring(begin.Length).Split(',');
            
        }
    }
}
