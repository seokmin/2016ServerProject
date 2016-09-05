using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;

namespace LoginServer.Data
{
    class UserRepository
    {
        public static async Task<DataTable> GetUser(string username)
        {
            var data = await DB.MysqlLib.SearchUser(username);

            return data;
        }

        public static async Task<Tuple<int, int>> CreateUser(string username, string pw)
        {
            Random r = new Random();
            int pokemon = r.Next(1, 151);

            int startChip = 50;

            return await DB.MysqlLib.CreateUser(username, pw, pokemon, startChip);
        }

        public static async Task<int> SaveAuthToken(string authToken, string username)
        {
            var timestamp = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

            return await DB.MysqlLib.SaveAuthToken(authToken, username, timestamp);
        }

        public static async Task<RES_LOGIN_CHANNEL[]> GetChannel()
        {
            var data = await DB.MysqlLib.GetChannel();

            RES_LOGIN_CHANNEL[] channel = new RES_LOGIN_CHANNEL[data.Rows.Count];

            for (int i = 0; i < data.Rows.Count; ++i)
            {
                channel[i].Name = data.Rows[i]["name"].ToString();
                channel[i].Rgb.r = (short)Int32.Parse(data.Rows[i]["r"].ToString());
                channel[i].Rgb.g = (short)Int32.Parse(data.Rows[i]["g"].ToString());
                channel[i].Rgb.b = (short)Int32.Parse(data.Rows[i]["b"].ToString());
                channel[i].IP = data.Rows[i]["ip"].ToString();
                channel[i].Port = Int32.Parse(data.Rows[i]["port"].ToString());
                channel[i].MinBet = Int32.Parse(data.Rows[i]["minBet"].ToString());
                channel[i].MaxBet = Int32.Parse(data.Rows[i]["maxBet"].ToString());
            }
            return channel;
        }

        public static async Task<ERROR_CODE> Logout(string authToken)
        {
            var response = await DB.MysqlLib.Logout(authToken);

            if (response != 1)
                return ERROR_CODE.REQ_LOGOUT_FAIL;

            return ERROR_CODE.NONE;
        }
    }
}
