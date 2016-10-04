using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Microsoft.Owin.Hosting;
using System.Web.Http;
using System.Xml;


namespace LoginServer
{
    public class Program
    {
        public static int StartMoney = 0;
        public static int AuthLastTime = 25;

        static void Main()
        {
            StartOptions options = new StartOptions();
            options.Urls.Add("http://*:8258");

            // Start OWIN host 
            using (WebApp.Start<Startup>(options))
            {
                Console.WriteLine("LoginServer Running.. : " + Environment.MachineName);

                XmlTextReader reader = new XmlTextReader("LoginServerConfig.xml");

                string element = "";
                while (reader.Read())
                {
                    if(reader.NodeType == XmlNodeType.Element)
                    { 
                        element = reader.Name;
                        continue;
                    }

                    if (reader.NodeType == XmlNodeType.Text)
                    {
                        switch(element)
                        {
                            case "StartMoney":
                                StartMoney = Int32.Parse(reader.Value);
                                break;

                            case "AuthLastTime":
                                AuthLastTime = Int32.Parse(reader.Value);
                                break;
                        }
                    }
                }
                Console.WriteLine("Start Money is : " + StartMoney + ", Auth last time is : " + AuthLastTime);

                DB.MysqlLib.ClearAuthToken();
                Console.WriteLine("이미 존재하는 Auth Token들 지움..");

                DB.MysqlLib.ClearAuthTokenLoop(AuthLastTime);

                //이 using 안에서 빠져나가면 서버 종료됨..
                Console.ReadLine();
            }

        }
    }
}
