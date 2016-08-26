using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Web.Http;

namespace LoginServer.Request
{
    public class LoginController : ApiController
    {
        [Route("Request/Login")]
        [HttpPost]
        public async Task<RES_LOGIN> Process(REQ_LOGIN requestPacket)
        {
            var responseResult = new RES_LOGIN();

            var userData = await Data.UserRepository.GetUser(requestPacket.UserID);
            // user가 없다면 새로운 유저를 생성해줌.

            if (userData == null)
            {
                responseResult.Return(ERROR_CODE.REQ_LOGIN_GET_USER_ERROR);
            }

            if (userData.Rows.Count == 0)
            {
                var createResult = await Data.UserRepository.CreateUser(requestPacket.UserID, requestPacket.PW);
                if (createResult.Item1 != 1)
                {
                    responseResult.Return(ERROR_CODE.REQ_LOGIN_CREATE_ERROR);
                }

                responseResult.Pokemon = (short)createResult.Item2;
                responseResult.Chip = 300;
                responseResult.SetResult(ERROR_CODE.REQ_LOGIN_CREATE_SUCCESS);
            }
            else
            {
                if (userData.Rows[0]["pw"].ToString() != requestPacket.PW)
                {
                    return responseResult.Return(ERROR_CODE.REQ_LOGIN_INVALID_PW);
                }
                responseResult.Pokemon = (short)Int32.Parse(userData.Rows[0]["pokemon"].ToString());
                responseResult.Chip = (short)Int32.Parse(userData.Rows[0]["chip"].ToString());
                responseResult.SetResult(ERROR_CODE.NONE);
            }

            responseResult.Channels = await Data.UserRepository.GetChannel();

            Guid g = Guid.NewGuid();
            string authToken = Convert.ToBase64String(g.ToByteArray());
            //authToken = authToken.Replace("=", "");
            //authToken = authToken.Replace("+", "");
            //authToken = authToken.Replace("/", "");

            int affectedRows = await Data.UserRepository.SaveAuthToken(authToken, requestPacket.UserID);

            if (affectedRows == 0)
            {
                responseResult.Return(ERROR_CODE.REQ_LOGIN_AUTH_SAVE_ERROR);
            }

            responseResult.AuthToken = authToken;
            return responseResult;
        }



        [Route("Request/Logout")]
        [HttpPost]
        public async Task<RES_LOGOUT> Process(REQ_LOGOUT requestPacket)
        {
            var responseResult = new RES_LOGOUT();

            var response = await Data.UserRepository.Logout(requestPacket.AuthToken);

            if (response != ERROR_CODE.NONE)
                return responseResult.Return(response);

            responseResult.SetResult(response);

            return responseResult;
        }
    }
}
