using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;
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

        public AccountController(
            UserManager<GTKeeperAPI.Models.Identity.GTKeeperUser> userManager,
            SignInManager<GTKeeperAPI.Models.Identity.GTKeeperUser> signInManager,
            IConfiguration configuration
            )
        {
            _userManager = userManager;
            _signInManager = signInManager;
            _configuration = configuration;
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


        [HttpPost]
        public async Task<IActionResult>  ResetPassword(string email,string token,string newpassword)
        {
            var userInfo = await _userManager.FindByEmailAsync(email);

            if (userInfo != null)
            {

                if (token != userInfo.SecurityStamp)
                    return NotFound(new { code = 1991 });

                var result = await _userManager.ResetPasswordAsync(userInfo,token,newpassword);
                

            
                if (result.Succeeded)
                    return Ok();    
                else
                    return BadRequest();


            }
            return NotFound();
        }


        [HttpPost]
        public async Task<IActionResult> SendResetPassword(string email)
        {
            var userInfo = await _userManager.FindByEmailAsync(email);

            if (userInfo != null)
            {
                //Enviamos mail
                return Ok();
                 

            }
            return NotFound();
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
            var result = await _signInManager.PasswordSignInAsync(model.Email, model.Password, false, false);

            if (result.Succeeded)
            {

                // Returning a SignInResult will ask OpenIddict to issue the appropriate access/identity tokens.
                
                var userInfo = await _userManager.GetUserAsync(ClaimsPrincipal.Current);
                return Ok(new
                {
                    isAuthenticated = true,
                    signInResult = result,
                    userInfo = userInfo,
                    token= GenerateJwtToken ( model.Email,userInfo )
                });
                 
     
            }
            return Forbid();
        }

        [HttpPost]
        public async Task<IActionResult> Register([FromBody] RegisterDto model)
        {
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
                return Ok(await GenerateJwtToken(model.Email, user));
            }

            return Forbid();
        }

        private async Task<object> GenerateJwtToken(string email, IdentityUser user)
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

            return new JwtSecurityTokenHandler().WriteToken(token);
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
    }
}