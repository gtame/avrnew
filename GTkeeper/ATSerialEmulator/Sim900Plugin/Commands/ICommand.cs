using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sim900Plugin.Commands
{
    public interface ICommand
    {
        string ProcessCommand(string command);
    }

    public interface ICommandPattern
    {
        string Pattern { get; }
    }
}
