desc "Generate the site"
task :generate do
  `jekyll build`
end

desc "Deploy the site"
task :deploy => [:generate] do
  # `rsync -av _site/ seebq@seebq.com:/var/www/seebq.com/htdocs/`
  # `rsync -av -e "ssh -i /Users/cbq/.ssh/id-cbq_keypair" _site/ ec2-user@ec2-23-20-205-53.compute-1.amazonaws.com:/var/www/html/`
  `rsync -av -e "ssh -i /Users/cbq/.ssh/id-cbq_keypair" _site/ ec2-user@50.16.202.19:/var/www/seebq.com/`
end

desc "Run the server"
task :server do
  `jekyll serve --watch`
end
