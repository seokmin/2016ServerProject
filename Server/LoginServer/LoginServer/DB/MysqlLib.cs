using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;
using MySql.Data.MySqlClient;

namespace LoginServer.DB
{
    public static class MysqlLib
    {
        const string MYSQL_HOST = "localhost";
        const string MYSQL_DB = "test";
        const string MYSQL_USER_ID = "root";
        const string MYSQL_USER_PW = "vudghk3";

        const string MYSQL_CONNECT_STRING = "Server=" + MYSQL_HOST + ";Database=" + MYSQL_DB + ";Uid=" + MYSQL_USER_ID + ";Pwd=" + MYSQL_USER_PW;
        
        public static async Task<DataTable> SearchUser(string username)
        {
            DataTable dt = null;
            using (MySqlConnection myConnection = new MySqlConnection(MYSQL_CONNECT_STRING))
            {
                myConnection.Open();

                DataSet ds = new DataSet();
                MySqlDataAdapter da = new MySqlDataAdapter("SELECT username, pw FROM user where username = '" + username + "'", myConnection);
                await da.FillAsync(ds, "userInfo");

                dt = ds.Tables["userInfo"];

                myConnection.Close();
            }

            return dt;
        }
    }

    public class DBUser
    {
        public string username;
        public string PW;
    }
}
