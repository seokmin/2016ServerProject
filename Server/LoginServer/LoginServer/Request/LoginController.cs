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

            Console.WriteLine(userData);

            if (userData == null)
            {
                responseResult.Return(ERROR_CODE.REQ_LOGIN_GET_USER_ERROR);
            }

            if(userData.Rows.Count == 0)
            {
                int affectedRows = await Data.UserRepository.CreateUser(requestPacket.UserID, requestPacket.PW);
                if (affectedRows != 1)
                {
                    Console.WriteLine("errr" + affectedRows);
                    responseResult.Return(ERROR_CODE.REQ_LOGIN_CREATE_ERROR);
                }

                responseResult.SetResult(ERROR_CODE.REQ_LOGIN_CREATE_SUCCESS);
            }
            else
            {
                if (userData.Rows[0]["pw"].ToString() != requestPacket.PW)
                {
                    return responseResult.Return(ERROR_CODE.REQ_LOGIN_INVALID_PW);
                }
                responseResult.SetResult(ERROR_CODE.NONE);
            }

            var authToken = System.DateTime.Now.Ticks.ToString();

            responseResult.AuthToken = authToken;
            return responseResult;
        }
    }
}
