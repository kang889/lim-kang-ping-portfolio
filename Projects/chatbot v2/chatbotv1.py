import json
import requests
import spacy
from difflib import get_close_matches

nlp = spacy.load('en_core_web_sm')

API_KEY = 'YOUR_API_KEY'
BASE_URL = 'http://api.openweathermap.org/data/2.5/weather'


def load_knowledge_base(file_path: str) -> dict:
    with open(file_path, 'r') as file:
        data : dict = json.load(file)
    return data

def save_knowledge_base(file_path: str, data:dict):
    with open(file_path, 'w') as file:
        json.dump(data, file, indent = 2)

def find_best_match(user_question: str, questions: list[str]) -> str | None: 
    matches: list = get_close_matches(user_question, questions, n=1, cutoff=0.9)
    return matches[0] if matches else None

def get_answer_for_question(question: str, knowledge_base : dict) -> str | None:
    for q in knowledge_base['questions']:
        if q['question'] == question :
            return q['answer']

def extract_entities(user_input : str) -> list :
    doc =nlp(user_input)
    entities = [{'text': ent.text, 'label' : ent.label_} for ent in doc.ents]
    return entities

def update_weather(city : str) -> str: 
    params = {'q': city, 'appid':API_KEY}
    api_response = requests.get(BASE_URL, params = params)
    
    if api_response.status_code == 200:
        weather_data = api_response.json()

        if 'main' in weather_data and 'temp' in weather_data['main'] :
            temperature = weather_data['main']['temp']
            return f'the current weather in Singapore is {temperature}°c'
        else :
            return f'i couldn\'t retrieve the weather information for Singapore'
    return f'i couldn\'t retrive the weather information for singapore'

def chat_bot():
    knowledge_base : dict = load_knowledge_base('knowledge_base.json')
    while True :
        user_input = input('You: ')
        if user_input.lower() == 'quit':
            break
        entities = extract_entities(user_input)

        potential_questions = [f'{ent["text"]} ({ent["label"]})' for ent in entities] + [q['question'] for q in knowledge_base['questions']]

        best_match : str | None = find_best_match(user_input.lower(), potential_questions)

        if best_match:
            answer : str = get_answer_for_question(best_match, knowledge_base)
            print(f'YapBot: {answer}')
        elif 'weather' in user_input.lower(): 
            weather_response = update_weather('Singapore')
            print(f'YapBot: {weather_response}')
        else: 
            print('YapBot: I Don\'t know thehehello answer, can you teach me?')
            new_answer = input('Type the answer or "Skip" to skip: ')
            if new_answer.lower() != 'skip':
                knowledge_base['questions'].append({'question': user_input, 'answer':new_answer})
                save_knowledge_base('knowledge_base.json', knowledge_base)
                print('YapBot: Thank you ! i learned a new response !')
            
            

if __name__ == '__main__':
    chat_bot()











