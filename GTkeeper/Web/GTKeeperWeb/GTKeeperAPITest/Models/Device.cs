using NUnit.Framework;
using GTKeeperAPI.Models;
using System.IO;
using System.Reflection;

namespace GTKeeperAPITest.Models
{
    [TestFixture]
    public class Device_test
    {
        public Device GetDevice()
        {
            GTKeeperAPI.Models.Device device = new GTKeeperAPI.Models.Device()
            {
                Imei = "123456789012345",
                Nombre = "Test1",
                AvisosSMS = SmsAviso.SMSNone,
                MovilAviso = "653316799",
                MotorDiesel = false,
                NumAbono = 1,
                NumPuertos = 15

            };
            return device;
        }

        

        public static string ReadResource(string resourceName)
        {
            string result = null;
            var assembly = Assembly.GetExecutingAssembly();


            using (Stream stream = assembly.GetManifestResourceStream(resourceName))
            using (StreamReader reader = new StreamReader(stream))
            {
                result = reader.ReadToEnd();
            }

            return result;
        }

        
        [Test]
        [Category("ToString")]
        public void toString_isValid()
        {

            Device device = GetDevice();
            Assert.True("0006533167990115" == device.ToString());

        }


 

        
        [TestCase("solo_config.txt")]
        [TestCase("solo_programas.txt")]
        [TestCase("solo_log.txt")]
        [TestCase("mix.txt")]
        public void parseFile_isValid(string resourceName)
        {
            Device device = GetDevice();

            string resource = ReadResource($"GTKeeperAPITest.Resources.{resourceName}");

            Assert.True(device.ParseFile(resource.Split("\r\n"),"0","0"));
             

        }


    }
}
