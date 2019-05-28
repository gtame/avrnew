using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Mail;
using System.Threading.Tasks;

namespace GTKeeperAPI.Services
{
    public class SenderMailService:ISendMail
    {

         public SenderMailService(IOptions<EmailSettings> emailSettings, ILogger<SenderMailService> logger  )
        {
            _emailSettings = emailSettings.Value;
            _logger = logger;
        }

        public EmailSettings _emailSettings { get; }
        public ILogger<SenderMailService> _logger { get; }


        public Task SendEmailAsync(string email, string subject, string message)
        {

            Execute (email, subject, message).Wait();
            return Task.FromResult(0);
        }

        public async Task Execute(string email, string subject, string message)
        {
          try
          {
            string toEmail = string.IsNullOrEmpty(email) 
                             ? _emailSettings.ToEmail 
                             : email;
            MailMessage mail = new MailMessage()
            {
                From = new MailAddress(_emailSettings.UsernameEmail, _emailSettings.DisplayName)
            };
            mail.To.Add(new MailAddress(toEmail));

            if (!string.IsNullOrWhiteSpace(_emailSettings.CcEmail))
                mail.CC.Add(new MailAddress(_emailSettings.CcEmail));

            mail.Subject =   subject;
            mail.Body = message;
            mail.IsBodyHtml = true;
            mail.Priority = MailPriority.High;

            using (SmtpClient smtp = new SmtpClient(_emailSettings.SecondayDomain, _emailSettings.SecondaryPort))
            {
                smtp.Credentials = new NetworkCredential(_emailSettings.UsernameEmail, _emailSettings.UsernamePassword);
                smtp.EnableSsl = _emailSettings.EnableSsl;
                await smtp.SendMailAsync(mail);
            }
          }
          catch(Exception ex)
          {
                //do something here
                _logger.LogError(ex, "error enviando mail");
          }
        }
    }
}
