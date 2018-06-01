using NUnit.Framework;
using GTKeeperAPI.Models;

namespace GTKeeperAPITest.Models
{
    [TestFixture]
    public class Device_test
    {
        [Test]
        [Category("ToString")]
        public void toString_isValid()
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

            Assert.True("0006533167990115" == device.ToString());

        }


    }
}
