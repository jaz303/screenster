require './handlers'

PARAM_TYPES = {

  :Q => {
    :c_type => 'uint64_t', :size => 8, :reader => 'READ_UINT64()'
  },
  :L => {
    :c_type => 'uint32_t', :size => 4, :reader => "READ_UINT32()"
  },
  :S => {
    :c_type => 'uint16_t', :size => 2, :reader => "READ_UINT16()"
  },
  :C => {
    :c_type => 'unsigned char', :size => 1, :reader => "READ_BYTE()"
  },

  :q => {
    :c_type => 'int64_t', :size => 8, :reader => 'READ_UINT64()'
  },
  :l => {
    :c_type => 'int32_t', :size => 4, :reader => "READ_UINT32()"
  },
  :s => {
    :c_type => 'int16_t', :size => 2, :reader => "READ_UINT16()"
  },
  :c => {
    :c_type => 'char', :size => 1, :reader => "READ_CHAR()"
  }

}

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
        
        handler_source_file = "src/commands/#{group}/#{fun}.c"
        unless File.exists?(handler_source_file)
          FileUtils.mkdir_p(File.dirname(handler_source_file))
          File.open(handler_source_file, "w") do |f|
            f.write("/* stub handler */\n")
          end
        end

        handler_source = ""
        command_params = params[:params] || []
        command_params_size = 0
        
        ix = 0
        while ix < command_params.length
          param_type = PARAM_TYPES[command_params[ix]]
          param_name = command_params[ix+1].gsub('-', '_')
          command_params_size += param_type[:size]
          handler_source << "#{param_type[:c_type]} #{param_name} = #{param_type[:reader]};\n"
          ix += 2
        end

        size_check  = "if (cmd_len != #{command_params_size}) {\n"
        size_check << "    // TODO: handle error\n"
        size_check << "    return;\n"
        size_check << "}\n\n"

        handler_source = size_check + handler_source + "\n" + File.read(handler_source_file).strip
        
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