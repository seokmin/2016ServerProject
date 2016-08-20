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
        public int Chip;
        public RES_LOGIN_CHANNEL[] Channels;
    }

    public struct Rgb
    {
        public short r;
        public short g;
        public short b;
    }

    public struct RES_LOGIN_CHANNEL
    {
        public string Name;
        public Rgb Rgb;
        public string IP;
        public short Port;
        public int MinBet;
        public int MaxBet;
    }

    public struct REQ_LOGOUT
    {
        public string AuthToken;
    }

    public struct RES_LOGOUT
    {
        public RES_LOGOUT Return(ERROR_CODE error)
        {
            Result = (short)error; return this;
        }

        public void SetResult(ERROR_CODE error)
        {
            Result = (short)error;
        }
        public short Result;
    }
    #endregion
}
