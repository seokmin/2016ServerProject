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
        
        REQ_LOGIN_INVALID_PW = 10,
        REQ_LOGIN_CREATE_ERROR = 11,
        REQ_LOGIN_CREATE_SUCCESS = 12,
        REQ_LOGIN_GET_USER_ERROR = 13,
        REQ_LOGIN_AUTH_SAVE_ERROR = 13,

    }
}
