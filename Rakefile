require './handlers'

def c_function(command)
  'cmd_' + command.downcase.gsub('-', '_')
end

task :generate_handlers do
  
  handlers = ""
  init_function = "void handlers_init() {\n"
  
  HANDLERS.each do |group,categories|
    categories.each do |category,commands|
      init_function << "    create_category(#{category}, #{commands.length});\n"
      commands.each do |command,params|
        fun = c_function(params[:name])
        
        init_function << "    install_handler(#{category}, #{command}, #{fun});\n"
        
        handler_source = "src/commands/#{group}/#{fun}.c"
        unless File.exists?(handler_source)
          FileUtils.mkdir_p(File.dirname(handler_source))
          File.open(handler_source, "w") do |f|
            f.write("/* stub handler */\n")
          end
        end
        
        handlers << "HANDLER_FN(#{fun}) {\n"
        handlers << File.read(handler_source).strip.gsub(/^/, '    ')
        handlers << "\n}\n\n"
      end
    end
  end
  
  init_function << "}\n\n"
  
  File.open("src/handlers.c", "w") do |f|
    f.write "/* This file is auto-generated (run `rake generate_handlers`) */\n\n"
    f.write handlers
    f.write init_function
  end
end