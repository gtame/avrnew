using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Security.Claims;
using System.Text;
using System.Text.Encodings.Web;
using System.Threading.Tasks;
using System.Web;
using GTKeeperAPI.Services;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using Microsoft.IdentityModel.Tokens;

namespace GTKeeperAPI.Controllers
{
    [Route("[controller]/[action]")]
    public class AccountController : Controller
    {
        private readonly SignInManager<GTKeeperAPI.Models.Identity.GTKeeperUser> _signInManager;
        private readonly UserManager<GTKeeperAPI.Models.Identity.GTKeeperUser> _userManager;
        private readonly IConfiguration _configuration;
        private readonly ISendMail _emailSender;

        public AccountController(
            UserManager<GTKeeperAPI.Models.Identity.GTKeeperUser> userManager,
            SignInManager<GTKeeperAPI.Models.Identity.GTKeeperUser> signInManager,
            IConfiguration configuration,
            ISendMail emailSender
            )
        {
            _userManager = userManager;
            _signInManager = signInManager;
            _configuration = configuration;
            _emailSender = emailSender;
        }


        [Route("~/account/detail")]
        [HttpPost]
        public async Task<IActionResult> GetUserDetail(string email)
        {
            var userInfo = await _userManager.FindByEmailAsync(email);
            if (userInfo != null)               
                return Ok(userInfo);
            else
                return NotFound();
        }

        [HttpPost]
        public async Task<IActionResult> ChangePassword(string email, string oldpassword, string newpassword)
        {
            var userInfo = await _userManager.FindByEmailAsync(email);

            if (userInfo != null)
            {
                var result = await _userManager.ChangePasswordAsync(userInfo, oldpassword, newpassword);

                if (result.Succeeded)
                    return Ok();
                else
                    return BadRequest();
            }
            return NotFound();
        }

        [Route("~/account/reset-pass")]
        [HttpPut]
        public async Task<IActionResult>  ResetPassword([FromBody]ResetPassDto model)
        {


            Response response = new Response();
             
            
            //get user by email
            var userInfo = _userManager.Users.SingleOrDefault( x => x.Id == model.User);

      
            if (userInfo != null && !string.IsNullOrEmpty(model.Token))
            {
                var result = await _userManager.ResetPasswordAsync(userInfo, model.Token, model.Password);

                if (result.Succeeded)
                {

                    response.Messages.Add("Password cambiada con exito");

                    return Ok(response);
                }
                else
                {
                    response.AddErrors(result.Errors);
                    return BadRequest(response);
                }
            }

            return NotFound();
        }


        [Route("~/account/request-pass")]
        [HttpPost]
        public async Task<IActionResult> RequestPassword([FromBody]LoginDto model)
        {
            var userInfo = await _userManager.FindByEmailAsync(model.Email);

            if (userInfo != null)
            {

                string token = await _userManager.GeneratePasswordResetTokenAsync(userInfo);

                var parametersToAdd = new System.Collections.Generic.Dictionary<string, string> { { "id", userInfo.Id },{ "token", token } };
                var someUrl = "http://localhost:4200/auth/reset-password";
                var newUri = Microsoft.AspNetCore.WebUtilities.QueryHelpers.AddQueryString(someUrl, parametersToAdd);


                await _emailSender.SendEmailAsync(userInfo.Email, "Request password",
                       $"Please confirm your account by <a href='{HtmlEncoder.Default.Encode(newUri)}'>clicking here</a>.");
                //Enviamos mail
                return Ok();
                 

            }
            return NotFound();
        }


        [Route("~/account/refreshToken")]
        [HttpPost]
        public async Task<IActionResult> RefreshToken( )
        {


            Response response = new Response();

            System.Security.Claims.ClaimsPrincipal currentUser = this.User;
            var userInfo = await _userManager.GetUserAsync(currentUser);

            if (userInfo != null)
            {
                response.Token = await GenerateJwtToken(userInfo.Email, userInfo);
             
                return Ok(response);
      
           
            }
            return NotFound();
        }



        [Route("~/account/logout")]
        [HttpDelete]
        public async Task<IActionResult> Logout()
        {
            System.Security.Claims.ClaimsPrincipal currentUser = this.User;
            var userInfo = await _userManager.GetUserAsync(currentUser);

            await _signInManager.SignOutAsync();

            if (!currentUser.Identity.IsAuthenticated)
            {

                //Enviamos mail
                return Ok();
            }
            else
                return BadRequest();

             
        }



        [HttpGet]
        public async Task<IActionResult> Activate(string email,string token)
        {
            var userInfo = await _userManager.FindByEmailAsync(email);

            if (userInfo!=null)
            {
                if (token != userInfo.SecurityStamp)
                    return NotFound(new { code = 1991 });

                //Check si account fue activada
                if (!userInfo.EmailConfirmed)
                {
                    userInfo.EmailConfirmed = true;
                    await _userManager.UpdateAsync(userInfo);
                    return Ok();

                }
                else
                    return BadRequest();

            }
            return NotFound();
        }

        [HttpPost]
        public async Task<IActionResult> Login([FromBody] LoginDto model)
        {
            Response response = new Response();
            var result = await _signInManager.PasswordSignInAsync(model.Email, model.Password, false, false);

            if (result.Succeeded)
            {
                // Returning a SignInResult will ask OpenIddict to issue the appropriate access/identity tokens.
                var userInfo = await _userManager.FindByEmailAsync(model.Email);
                response.Token = await GenerateJwtToken(model.Email, userInfo);
                
                return Ok(response);
            }
            else
            {
                var userInfo = await _userManager.FindByEmailAsync(model.Email);
                if (userInfo==null)
                    return Forbid();
                else
                {
                    if (result.IsLockedOut)
                    {
                          response.Errors.Add("Usuario bloqueado");
                    }
                    else if (result.IsNotAllowed)
                    {
                        if (!userInfo.EmailConfirmed)
                            response.Errors.Add("El usuario esta pendiente de activacion");
                    }

                    return NotFound(response);
                }
            }
            
        }

        [HttpPost]
        public async Task<IActionResult> Register([FromBody] RegisterDto model)
        {

            Response response = new Response();

            var user = new GTKeeperAPI.Models.Identity.GTKeeperUser
            {
                UserName = model.Email,
                Email = model.Email,
                FirstName = "test",
                LastName = "test2",
                Level = 1,
                JoinDate=System.DateTime.Now
            };
            var result = await _userManager.CreateAsync(user, model.Password);

            if (result.Succeeded)
            {
                await _signInManager.SignInAsync(user, false);

                response.Token = await GenerateJwtToken(model.Email, user);
                response.Messages.Add("Todo fue OK!");

                return Ok(response);
            }

            response.AddErrors(result.Errors);
            return NotFound(new
            {
                data = response
            });
        }

        private async Task<string> GenerateJwtToken(string email, IdentityUser user)
        {
            var claims = new List<Claim>
            {
                new Claim(JwtRegisteredClaimNames.Sub, email),
                new Claim(JwtRegisteredClaimNames.Jti, Guid.NewGuid().ToString()),
                new Claim(ClaimTypes.NameIdentifier, user.Id)
            };

            var key = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(_configuration["JwtKey"]));
            var creds = new SigningCredentials(key, SecurityAlgorithms.HmacSha256);
            var expires = DateTime.Now.AddDays(Convert.ToDouble(_configuration["JwtExpireDays"]));

            var token = new JwtSecurityToken(
                _configuration["JwtIssuer"],
                _configuration["JwtIssuer"],
                claims,
                expires: expires,
                signingCredentials: creds
            );

            token.Payload.Add("user", user);
            

            return new JwtSecurityTokenHandler().WriteToken(token);
        }


        public class Response
        {

            private List<string> _messages;
            private List<string> _errors;
            [Required]
            public string Token { get; set; }
             
        
            public List<string> Messages { get
                {
                    if (_messages == null)
                        _messages = new List<string>();
                    return _messages;
                }
            }


            public List<string> Errors
            {
                get
                {
                    if (_errors == null)
                        _errors = new List<string>();
                    return _errors;
                }
            }

            public void AddErrors(IEnumerable<IdentityError> errors)
            {
                foreach (var err in errors)
                    Errors.Add(err.Description);
            }

        }

        public class LoginDto
        {
            [Required]
            public string Email { get; set; }

            [Required]
            public string Password { get; set; }

        }

        public class RegisterDto
        {
            [Required]
            public string Email { get; set; }

            [Required]
            [StringLength(100, ErrorMessage = "PASSWORD_MIN_LENGTH", MinimumLength = 6)]
            public string Password { get; set; }
        }


        public class ResetPassDto
        {

            public string User { get; set; }

            public string Password { get; set; }
            public string Token { get; set; }
        }
    }
}
