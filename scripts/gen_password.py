import bcrypt

password = b"password"

print(bcrypt.hashpw(password, bcrypt.gensalt()))
