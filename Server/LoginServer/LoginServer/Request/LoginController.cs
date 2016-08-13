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
            Console.WriteLine("dsfsdfdfsd");
            var responseResult = new RES_LOGIN();

            var userData = await Data.UserRepository.GetUser(requestPacket.UserID);
            if (userData == null)
            {
                return responseResult.Return(ERROR_CODE.REQ_LOGIN_INVALID_USERNAME);
            }

            if (userData.Rows[0]["pw"].ToString() != requestPacket.PW)
            {
                return responseResult.Return(ERROR_CODE.REQ_LOGIN_INVALID_PW);
            }

            var authToken = System.DateTime.Now.Ticks.ToString();

            responseResult.SetResult(ERROR_CODE.NONE);
            responseResult.AuthToken = authToken;
            return responseResult;
        }
    }
}
