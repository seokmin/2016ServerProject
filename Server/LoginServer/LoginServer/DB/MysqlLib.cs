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

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("SELECT username, pw, pokemon FROM user where username = '{0}'", username);

                MySqlDataAdapter da = new MySqlDataAdapter(sb.ToString(), myConnection);
                await da.FillAsync(ds, "userInfo");

                dt = ds.Tables["userInfo"];

                myConnection.Close();
            }

            return dt;
        }

        public static async Task<Tuple<int, int>> CreateUser(string username, string pw, int pokemon)
        {
            int affectedRows = 0;
            using (MySqlConnection myConnection = new MySqlConnection(MYSQL_CONNECT_STRING))
            {
                myConnection.Open();

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("INSERT INTO user SET username = '{0}', pw = '{1}', pokemon = '{2}'", username, pw, pokemon);

                MySqlCommand cmd = new MySqlCommand(sb.ToString(), myConnection);
                affectedRows = await cmd.ExecuteNonQueryAsync();

                myConnection.Close();
            }

            return new Tuple<int, int>(affectedRows, pokemon);
        }

        public static async Task<int> SaveAuthToken(string authToken, string username, string timestamp)
        {
            int affectedRows = 0;
            using (MySqlConnection myConnection = new MySqlConnection(MYSQL_CONNECT_STRING))
            {
                myConnection.Open();

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("INSERT INTO auth SET username = '{0}', authToken = '{1}', timestamp = '{2}'", username, authToken, timestamp);

                MySqlCommand cmd = new MySqlCommand(sb.ToString(), myConnection);
                affectedRows = await cmd.ExecuteNonQueryAsync();

                myConnection.Close();
            }

            return affectedRows;
        }

        public static async Task<DataTable> GetChannels()
        {
            DataTable dt = null;
            using (MySqlConnection myConnection = new MySqlConnection(MYSQL_CONNECT_STRING))
            {
                myConnection.Open();

                DataSet ds = new DataSet();

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("SELECT ip, curNum, maxNum, timestamp FROM channel");

                MySqlDataAdapter da = new MySqlDataAdapter(sb.ToString(), myConnection);
                await da.FillAsync(ds, "channelInfo");

                dt = ds.Tables["channelInfo"];

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
