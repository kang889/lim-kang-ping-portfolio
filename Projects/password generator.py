import random

# Character groups
characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()_+/?><'
capletters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
lowerletters = 'abcdefghijklmnopqrstuvwxyz'
specialchar = '!@#$%^&*()_+'
nums = '0123456789'

# Keep generating until criteria are met
while True:
    password = ''.join(random.choices(characters, k=16))
    if (any(char in capletters for char in password) and
        any(char in lowerletters for char in password) and
        any(char in specialchar for char in password) and
        any(char in nums for char in password)):
        print('Secure Password:', password)
        break
