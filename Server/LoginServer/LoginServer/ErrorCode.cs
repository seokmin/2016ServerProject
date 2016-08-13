using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LoginServer
{
    public enum ERROR_CODE
    {
        NONE = 0,

        REQ_LOGIN_INVALID_USERNAME = 10,
        REQ_LOGIN_INVALID_PW = 11,
    }
}
