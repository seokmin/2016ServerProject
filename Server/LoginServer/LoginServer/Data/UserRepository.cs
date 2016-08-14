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

            return await DB.MysqlLib.CreateUser(username, pw, pokemon);
        }

        public static async Task<int> SaveAuthToken(string authToken, string username)
        {
            var timestamp = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

            return await DB.MysqlLib.SaveAuthToken(authToken, username, timestamp);
        }

        public static async Task<RES_LOGIN_CHANNEL[]> GetChannels()
        {
            var data = await DB.MysqlLib.GetChannels();

            RES_LOGIN_CHANNEL[] channel = new RES_LOGIN_CHANNEL[data.Rows.Count];

            for(int i=0; i<data.Rows.Count; ++i)
            {
                channel[i].IP = data.Rows[i]["ip"].ToString();
                channel[i].CurNum = (short)Int32.Parse(data.Rows[i]["curNum"].ToString());
                channel[i].MaxNum = (short)Int32.Parse(data.Rows[i]["maxNum"].ToString());
                channel[i].IsOpen = true;
            }

            return channel;
        }
    }
}
