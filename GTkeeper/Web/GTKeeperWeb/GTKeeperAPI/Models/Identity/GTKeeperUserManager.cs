using Microsoft.AspNetCore.Identity;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GTKeeperAPI.Models.Identity
{
    public class GTKeeperUserManager : UserManager<GTKeeperUser>
    {

        public GTKeeperUserManager(IUserStore<GTKeeperUser> store, IOptions<IdentityOptions> optionsAccessor, IPasswordHasher<GTKeeperUser> passwordHasher, IEnumerable<IUserValidator<GTKeeperUser>> userValidators, IEnumerable<IPasswordValidator<GTKeeperUser>> passwordValidators, ILookupNormalizer keyNormalizer, IdentityErrorDescriber errors, IServiceProvider services, ILogger<UserManager<GTKeeperUser>> logger):base(store,optionsAccessor,passwordHasher,userValidators,passwordValidators,keyNormalizer,errors,services,logger)
        {
            
        }
    }
}
