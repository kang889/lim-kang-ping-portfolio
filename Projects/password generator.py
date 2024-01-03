import random

characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()_+/?><'

password = ''

for x in range(16):
    password += random.choice(characters)


print('Password:', password)
