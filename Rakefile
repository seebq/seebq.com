desc "Generate the site"
task :generate do
  `jekyll`
end

desc "Deploy the site"
task :deploy => [:generate] do
  `rsync -av _site/ seebq@seebq.com:/var/www/seebq.com/htdocs/`
end

desc "Run the server"
task :server do
  `jekyll --server --auto`
end
