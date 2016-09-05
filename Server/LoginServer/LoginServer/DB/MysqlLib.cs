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
        const string MYSQL_HOST = "10.73.43.87";
        const string MYSQL_DB = "jackblack";
        const string MYSQL_USER_ID = "next";
        const string MYSQL_USER_PW = "1234";

        const string MYSQL_CONNECT_STRING = "Server=" + MYSQL_HOST + ";Database=" + MYSQL_DB + ";Uid=" + MYSQL_USER_ID + ";Pwd=" + MYSQL_USER_PW;
        
        public static async Task<DataTable> SearchUser(string username)
        {
            DataTable dt = null;
            using (MySqlConnection myConnection = new MySqlConnection(MYSQL_CONNECT_STRING))
            {
                myConnection.Open();

                DataSet ds = new DataSet();

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("SELECT username, pw, pokemon, chip FROM user where username = '{0}'", username);

                MySqlDataAdapter da = new MySqlDataAdapter(sb.ToString(), myConnection);
                await da.FillAsync(ds, "userInfo");

                dt = ds.Tables["userInfo"];

                myConnection.Close();
            }

            return dt;
        }

        public static async Task<Tuple<int, int>> CreateUser(string username, string pw, int pokemon, int startChip)
        {
            int affectedRows = 0;
            using (MySqlConnection myConnection = new MySqlConnection(MYSQL_CONNECT_STRING))
            {
                myConnection.Open();

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("INSERT INTO user SET username = '{0}', pw = '{1}', pokemon = '{2}', chip = '{3}'", username, pw, pokemon, startChip);

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

        public static async Task<DataTable> GetChannel()
        {
            DataTable dt = null;
            using (MySqlConnection myConnection = new MySqlConnection(MYSQL_CONNECT_STRING))
            {
                myConnection.Open();

                DataSet ds = new DataSet();

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("SELECT name, ip, port, r, g, b, minBet, maxBet FROM channel WHERE timestamp > DATE_SUB(NOW(), INTERVAL 30 SECOND)");

                MySqlDataAdapter da = new MySqlDataAdapter(sb.ToString(), myConnection);
                await da.FillAsync(ds, "channelInfo");

                dt = ds.Tables["channelInfo"];

                myConnection.Close();
            }

            return dt;
        }

        public static async Task<int> Logout(string authToken)
        {
            int affectedRows = 0;
            using (MySqlConnection myConnection = new MySqlConnection(MYSQL_CONNECT_STRING))
            {
                myConnection.Open();

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("DELETE FROM auth WHERE authToken = '{0}'", authToken);

                MySqlCommand cmd = new MySqlCommand(sb.ToString(), myConnection);
                affectedRows = await cmd.ExecuteNonQueryAsync();

                myConnection.Close();
            }

            return affectedRows;
        }

        public static async void ClearAuthToken()
        {
            using (MySqlConnection myConnection = new MySqlConnection(MYSQL_CONNECT_STRING))
            {
                myConnection.Open();

                DataSet ds = new DataSet();

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("TRUNCATE auth");

                MySqlCommand cmd = new MySqlCommand(sb.ToString(), myConnection);
                await cmd.ExecuteNonQueryAsync();


                myConnection.Close();
            }
        }
    }

    public class DBUser
    {
        public string username;
        public string PW;
    }
}
