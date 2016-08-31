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

        static void Main()
        {
            StartOptions options = new StartOptions();
            options.Urls.Add("http://*:8258");

            // Start OWIN host 
            using (WebApp.Start<Startup>(options))
            {
                Console.WriteLine("LoginServer Running.. : " + Environment.MachineName);

                XmlTextReader reader = new XmlTextReader("LoginServerConfig.xml");
                while (reader.Read())
                {
                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Text: //Display the text in each element.
                            StartMoney = Int32.Parse(reader.Value);
                            Console.WriteLine("Start Money is ... " + reader.Value);
                            break;
                    }
                }

                DB.MysqlLib.ClearAuthToken();
                Console.WriteLine("이미 존재하는 Auth Token들 지움..");

                //이 using 안에서 빠져나가면 서버 종료됨..
                Console.ReadLine();
            }

        }
    }
}
