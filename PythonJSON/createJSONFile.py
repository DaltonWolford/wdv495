#!C:\Users\DaltonLaptop\AppData\Local\Programs\Python\Python38-32\python.exe

print("")
print("JSON Created")
import json

data = {
    'student_major': 'Web Development',
    'student_id': '332443',
    'student_gpa': '3.6',
	'student_courses': ['WDV101','WDV131', 'WDV105']
}


with open('student.json', 'w') as outfile:
    json.dump(data, outfile)