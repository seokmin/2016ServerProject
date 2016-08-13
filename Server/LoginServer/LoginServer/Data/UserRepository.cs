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
    }
}
