import re, os, urllib, html
import yaml, requests

def request_get(url):
	while True:
		try:
			req = requests.get(url, timeout=0.5)
			return req
		except:
			pass

class Commit:
	def output(self):
		print('[{id}] [language: {lang}] {source}'.format(
			id = self.id,
			lang = self.language,
			source = re.sub(r'[\s]*?', '', self.source)[0:100])
		)
	def __init__(self, id, language):
		self.id = id
		self.language = language
		url = 'http://uoj.ac/submission/{id}'.format(id=id)
		req = request_get(url)
		text = req.text
		source = re.findall(r'<pre><code class="[\s\S]*?</code></pre>', text)[0]
		source = re.sub(r'<pre><code class="[\s\S]*?">', '', source).replace('</code></pre>', '')
		self.source = html.unescape(source)

def download_commit(problem, page=1, language='c++'):
	global config
	global commitend
	url = "http://uoj.ac/submissions?problem_id={id}&min_score=100&max_score=100&language={lang}&page={page}".format(
		id = problem,
		page = page,
		lang = urllib.parse.quote(language.encode())
	)
	req = request_get(url)
	text = req.text
	commits = [re.findall(r'[0-9][0-9]*', it)[0] for it in re.findall(r'<a href="/submission/[0-9]*">#[0-9]*</a>', text)]
	answer = []
	for commit in commits:
                if int(commit) >= config['from']:
                        commitend=False
                if int(commit) >= config['from'] and int(commit) <= config['to']:
                        answer.append(Commit(commit, language))
	return answer

def judge(source, language):
	global config
	if os.path.exists('data.out'):
                os.remove('data.out')
	with open('source.cpp', 'w+', encoding='utf-8') as source_file:
		source_file.write(source)
		source_file.close()
	command = config['language'][language]['compile'].format(
		source = os.path.abspath('source.cpp'),
		program = os.path.abspath('data.exe'),
		input = os.path.abspath('data.in'),
		output = os.path.abspath('data.out')
	)
	os.system(command)
	if not os.system('fc data.out data.ans /w'):
		return False
	return True

def init():
	global config
	global commitend
	config = yaml.load(open('config.yml', 'r+', encoding="utf8").read())
	commits = []
	for language in config['language']:
		for page in range(1, 100):
                        commitend=True
                        answer = download_commit(config['problem'], page, language)
                        if commitend:
                                break
                        for commit in answer:
                                commit.output()
                                status = judge(commit.source, commit.language)
                                if status:
                                        print("http://uoj.ac/submission/"+commit.id)
                                        input('Incorrect!')
                        commits.extend(answer)

if __name__ == '__main__':
	init()
