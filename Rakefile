require './handlers'

PARAM_TYPES = {

  :Q => { :c_type => 'uint64_t', :reader => 'READ_FIXED_LENGTH(uint64_t, uint64)' },
  :L => { :c_type => 'uint32_t', :reader => 'READ_FIXED_LENGTH(uint32_t, uint32)' },
  :S => { :c_type => 'uint16_t', :reader => 'READ_FIXED_LENGTH(uint16_t, uint16)' },
  :C => { :c_type => 'unsigned char', :reader => 'READ_FIXED_LENGTH(unsigned char, byte)' },

  :q => { :c_type => 'int64_t', :reader => 'READ_FIXED_LENGTH(int64_t, int64)' },
  :l => { :c_type => 'int32_t', :reader => 'READ_FIXED_LENGTH(int32_t, int32)' },
  :s => { :c_type => 'int16_t', :reader => 'READ_FIXED_LENGTH(int16_t, int16)' },
  :c => { :c_type => 'char', :reader => "READ_FIXED_LENGTH(char, byte)" },

  :f => { :c_type => 'float', :reader => 'READ_FIXED_LENGTH(float, float)' },
  :d => { :c_type => 'double', :reader => 'READ_FIXED_LENGTH(double, double)' },
  
  :A => { :c_type => 'char*', :reader => 'READ_STRING()' }

}

def c_function(command)
  'cmd_' + command.downcase.gsub('-', '_')
end

task :generate_handlers do
  
  handlers = ""
  init_function = "void handlers_init() {\n"
  
  HANDLERS.each do |group,categories|
    categories.each do |category,commands|
      init_function << "    create_category(#{category}, #{commands.keys.max});\n"
      commands.each do |command,params|
        fun = c_function(params[:name])
        
        init_function << "    install_handler(#{category}, #{command}, #{fun});\n"
        
        handler_source_file = "src/commands/#{group}/#{fun}.c"
        unless File.exists?(handler_source_file)
          FileUtils.mkdir_p(File.dirname(handler_source_file))
          File.open(handler_source_file, "w") do |f|
            f.write("/* stub handler */\n")
          end
        end

        handler_source = ""
        command_params = params[:params] || []
        
        ix = 0
        while ix < command_params.length
          param_type = PARAM_TYPES[command_params[ix]]
          param_name = command_params[ix+1].gsub('-', '_')
          handler_source << "#{param_type[:c_type]} #{param_name} = #{param_type[:reader]};\n"
          ix += 2
        end

        handler_source = handler_source + "\n" + File.read(handler_source_file).strip
        
        handler_source << "\n\nreturn;\n"
        handler_source << "\n"
        handler_source << "param_error:\n"
        handler_source << "fprintf(stderr, \"parameter error!\\n\");\n"
        handler_source << "return;"
        
        handlers << "HANDLER_FN(#{fun}) {\n"
        handlers << handler_source.gsub(/^/, '    ')
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