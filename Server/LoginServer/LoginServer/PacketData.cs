using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LoginServer
{
    #region REQ_LOGIN
    public struct REQ_LOGIN
    {
        public string UserID;
        public string PW;
    }

    public struct RES_LOGIN
    {
        public RES_LOGIN Return(ERROR_CODE error)
        {
            Result = (short)error; return this;
        }

        public void SetResult(ERROR_CODE error)
        {
            Result = (short)error;
        }

        public short Result;
        public string AuthToken;
        public short Pokemon;
    }
    #endregion
}
